#include <stdio.h>
#include <windows.h>

DWORD WINAPI Thread1(LPVOID);
int stop;
int sleep = 10000;
struct params {
    int num;
    bool* runflg;
};

long long counters[7] = { 0, 0, 0, 0, 0, 0, 0 }; //счетчики для потоков
int priority[7] = {THREAD_PRIORITY_IDLE,THREAD_PRIORITY_LOWEST,
THREAD_PRIORITY_BELOW_NORMAL,THREAD_PRIORITY_NORMAL,
THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST,
THREAD_PRIORITY_TIME_CRITICAL}; //массив приоритетов в порядке возрастания

int main(int argc, char* argv[]) { //в командной строке аргументом задаем время жизни потоков
    stop = 10;
    DWORD targetThreadId;
    bool runFlag = true; //инициализация структур потока-таймера
    __int64 end_time;
    LARGE_INTEGER end_time2;
    HANDLE tm1 = CreateWaitableTimer(NULL, false, NULL); //создание таймера
    end_time = -1 * stop * 10000000;
    end_time2.LowPart = (DWORD) (end_time & 0xFFFFFFFF);
    end_time2.HighPart = (LONG) (end_time >> 32);
    SetWaitableTimer(tm1, &end_time2, 0,NULL, NULL, false);//запуск таймера
    for (int i = 0; i < 7; i++) {
        params* param = (params*)malloc(sizeof(params));
        param->num = i;
        param->runflg = &runFlag;
        HANDLE t1 = CreateThread(NULL, 0, Thread1, param, 0, &targetThreadId); //порождение потока и
        SetThreadPriority(t1, priority[i]); //задание ему приоритета
        PBOOL ptr1 = (PBOOL)malloc(sizeof(BOOL));
        //GetThreadPriorityBoost(t1, ptr1);
        //SetThreadPriorityBoost(t1, true); //проверка динамического распределения приоритетов
        CloseHandle(t1); //очистка памяти
    }
    WaitForSingleObject(tm1,INFINITE); //ожидание потока таймера
    runFlag = false; //флаг завершения работы
    CloseHandle(tm1);
    printf("\n");
    for (int i = 0; i < 7; i++) {
        printf("%d - %ld\n",i, counters[i]); //вывод результатов
    }
    return 0;
}

DWORD WINAPI Thread1(LPVOID prm) {
    while (1) {
        //DWORD WINAPI thrdid = GetCurrentThreadId(); //значение идентификатора вызывающего потока
        //HANDLE WINAPI handle = OpenThread(THREAD_QUERY_INFORMATION , false, thrdid); //дескриптор потока
        //int WINAPI prio = GetThreadPriority(handle); //приоритет для определяемого потока
        params arg = *((params*)prm);
        counters[arg.num]++;
        //if(prio != priority[arg.num])
        //    printf("\nPriority of %d is %d %d changed\n", arg.num, priority[arg.num], prio); //выводится когда динамическое распределение проиритетов включено
        //Sleep(0);
        if(*(arg.runflg) == false)
            break;
    }
    return 1;
}
