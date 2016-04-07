#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <Winsock2.h>
#include <list>  // Используем STL

using namespace std;  // Использовать пространство имен std
#define BUFF_SIZE  128  // Размер буфера
#define PORT 7500  // Номер порта
#define BUF_LEN 128
DWORD WINAPI ServerPool(HANDLE hp);

SOCKET server_sock;  // Прослушивающий сокет сервера
int ClientCount;  // Счетчик клиентов
list <SOCKET> ClientList;  // Список клиентов
//---------------------------------------------------------------------------
struct ovpConnection: public OVERLAPPED
{
    int client_number;  // Номер клиента
    SOCKET sock_handle;  // Сокет клиента
    char * buffer;  // Буфер сообщений
    enum
    {
        op_type_send,  // Посылка
        op_type_recv  // Прием
    }op_type;  // Тип операции
};
//---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    int err;  // Возвращаемое значение
    //  char buffer[128]; // Буфер для сообщений
    WORD wVersionRequested;  // Запрашиваемая версия
    WSADATA wsaData;  // Структура инф-ции о сокетах
    HANDLE hCp;  // Описатель порта завершения
    //  LPOVERLAPPED overlapped; // Структура асинхронного I/O
    HANDLE hThread;  // Хендл потока
    DWORD ThreadId;  // Идентификатор потока
    DWORD flags;  // Флаги ф-ции WSARecv
    //Инициализация библиотеки ws2_32.dll
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err == SOCKET_ERROR)
    {
        printf("Error on WSAStartup %d\n", WSAGetLastError());
        WSACleanup(); // Завершение работы
        _getch();
        return 2;
    }
    //Создаем порт завершения
    hCp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (hCp == NULL)
    {
        printf("Error on CreateIoCompletionPort %ld\n", GetLastError());
        WSACleanup(); // Завершение работы
        _getch();
        return 3;
    }
    // Задаем параметры для прослушивающего сокета сервера
    server_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL,
        WSA_FLAG_OVERLAPPED);
    if (server_sock == INVALID_SOCKET)
    {
        printf("Error on WSASocket %d\n", WSAGetLastError());
        WSACleanup(); //Завершение работы с сокетами
        _getch();
        return 4;
    }
    else
    {
        // Используем ранее созданный порт завершения
        if (CreateIoCompletionPort((HANDLE)server_sock, hCp, 0, 0) == NULL)
        {
            printf("Error on CreateIoCompletionPort %ld\n", GetLastError());
            WSACleanup(); //Завершение работы
            _getch();
            return 5;
        }
    }
    //Заполняем структуру адреса и подключаем сокет к коммуникационной среде
    SOCKADDR_IN sinServer;
    sinServer.sin_family = AF_INET;
    sinServer.sin_port = htons(PORT);
    sinServer.sin_addr.s_addr = INADDR_ANY;
    err = bind(server_sock, (LPSOCKADDR)&sinServer, sizeof(sinServer));
    if (err == -1)
    {
        printf("Error on bind %ld\n", GetLastError());
        WSACleanup(); //Завершение работы
        _getch();
        return 6;
    }
    //Создаем очередь для ожидания запросов от клиентов на соединение
    err = listen(server_sock, SOMAXCONN);
    if (err == -1) {
        printf("Error on listen № %ld\n", GetLastError());
        WSACleanup(); // Завершение работы
        _getch();
        return 7;
    }
    //Cоздаем рабочий поток для обслуживания сообщений от порта завершения
    for (int i=0; i < 8;i++)
        hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerPool, hCp, 0,
            &ThreadId);
    ClientCount = 0;
    printf("Server started \n");
    //Бесконечный цикл для многократного обслушивания запросов от клиентов
    while (true)
    {
        //Принимаем запрос от программы-клиента на установление связи
        SOCKADDR_IN sinClient;
        int lenClient = sizeof(sinClient);
        SOCKET client = accept(server_sock, (struct sockaddr*)&sinClient,
            &lenClient);
        CreateIoCompletionPort((HANDLE)client, hCp, 0, 0);
        //Добавляем клиента в список
        ClientList.insert(ClientList.end(), client);
        // Создаем overlapped-структуру
        ovpConnection * op = new ovpConnection;
        //Заполняем overlapped-структуру
        op->sock_handle = client;
        op->op_type = ovpConnection::op_type_recv;
        op->buffer = new char[BUFF_SIZE];
        op->hEvent = 0;
        op->client_number = ++ClientCount;
        printf("Client #%d connected, number of active clients "
            "%d\n",ClientCount,ClientList.size());
        unsigned long b;
        WSABUF buf;
        buf.buf = op->buffer;
        buf.len = BUFF_SIZE;
        flags = 0;
        err = WSARecv(op->sock_handle, &buf, 1, &b, &flags, op, 0);
        if (!err)
        {
            printf("Error on WSARecv %d\n", WSAGetLastError());
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
//Функция потока сервера для обслуживания порта завершения
//---------------------------------------------------------------------------
DWORD WINAPI ServerPool(HANDLE hp)
{
    int err;  // Возвращаемое значение
    unsigned long bytes;  // Кол-во байтов
    unsigned long key;  // Значение, асоциированное с хендлом порта
    char buffer[BUF_LEN];  // Буфер для сообщений
    LPOVERLAPPED overlapped; // Структура асинхронного I/O
    HANDLE hport = hp;  // Дескриптор порта
    DWORD flags;  // Флаги ф-ции WSARecv()
    ZeroMemory(buffer, BUF_LEN);
    while (true)
    {
        // Получаем информацию о завершении операции
        if (GetQueuedCompletionStatus(hport, &bytes, &key, &overlapped, INFINITE))
        {
            // Операция завершена успешно
            ovpConnection * op = (ovpConnection*)overlapped;
            // Определяем тип завершенной операции и выполняем соответствующие
            //действия
                switch (op->op_type)
            {
                //Завершена отправка данных
                case ovpConnection::op_type_send:
                    delete[] op->buffer;
                    delete op;
                    break;
                    //Завершен приём данных
                case ovpConnection::op_type_recv:
                    if (!bytes) {
                        //Соединение с данным клиентом закрыто
                        //ClientList.remove(op->sock_handle);
                        closesocket(op->sock_handle);
                        printf("Client # %d disconnected, number of active "
                            "clients %d\n",op->client_number,ClientList.size());
                            break;
                    }
                    op->buffer[bytes] = '\0';

                    printf("From client #%d recieved message %s\n", op->client_number, op->buffer);
                    if (send(op->sock_handle, op->buffer, bytes, 0) ==
                        SOCKET_ERROR) {
                        printf("error while sending: %d\n",
                            WSAGetLastError());
                    }

                    unsigned long b;
                    WSABUF buf;
                    buf.buf = op->buffer;
                    buf.len = BUFF_SIZE;  // buffer_len – постоянная величина
                    flags = 0;
                    err = WSARecv(op->sock_handle, &buf, 1, &b, &flags, op, 0);
                    if (!err)
                    {
                        printf("Error on WSARecv %d\n", WSAGetLastError());
                    }
            }
        }
        else
        {
            if (!overlapped)
            {
                // Ошибка с портом
                // Закрываем все сокеты, закрываем порт, очищаем список
                for (list<SOCKET>::iterator
                    i = ClientList.begin(); i != ClientList.end(); i++)
                {
                    closesocket(*i);
                }
                ClientList.clear();
                closesocket(server_sock);
                CloseHandle(hport);
                printf("Port error %ld, сервер завершает"
                    " работу\n",GetLastError());
                    _getch();
                exit(0);
            }
            else
            {
                //Закрываем соединение с клиентом
                closesocket(((ovpConnection*)overlapped)->sock_handle);
                //ClientList.remove(((ovpConnection*)overlapped)->sock_handle);
                printf("Client # %d disconnected, number of active clients "
                    "%d\n",((ovpConnection*)overlapped)->client_number, ClientList.size());
            }
        }
    }
    return 0;
}
