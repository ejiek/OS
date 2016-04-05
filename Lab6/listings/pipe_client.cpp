#include <stdio.h>
#include <Windows.h>

int main(int argc, char* argv[]) {
    char strtosend[100]; //строка для передачи
    char getbuf[100]; //буфер приема
    int bytestosend; //число передаваемых байт
    DWORD bytessended,bytesreaded; //число переданных и принятых байт
    for(int i=0;i<10;i++) {
        bytestosend=sprintf(strtosend,"message num %d",i+1);
        //формирование строки для
        //передачи
        strtosend[bytestosend]=0;
        fprintf(stderr,"client sended: \"%s\"\n",strtosend);
        if(!WriteFile(GetStdHandle(STD_OUTPUT_HANDLE),strtosend,bytestosend+1,&bytesreaded,NULL)) {
            fprintf(stderr,"Error with writeFile\n Wait 5 sec GetLastError= %d\n",GetLastError());
            Sleep(5000);
            return 1000;
        }
        if (!ReadFile(GetStdHandle(STD_INPUT_HANDLE), getbuf, 100, &bytesreaded, NULL)) {
            fprintf(stderr,"Error with readFile\n Wait 5 sec GetLastError= %d\n",GetLastError());
            Sleep(5000);
            return 1001;
        }
        fprintf(stderr,"Get msg from server: \"%s\"\n",getbuf);
    }
    fprintf(stderr,"client ended work\n Wait 5 sec");
    Sleep(5000);
    return 0;
}
