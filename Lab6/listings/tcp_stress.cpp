#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_STR_LEN 255
#define SIZE_OF_BUF 255

int recvn(SOCKET fd, char *bp, size_t len) { return recv(fd, bp, len, MSG_WAITALL); }

int sendn(SOCKET s, char* buf, int lenbuf, int flags) {
    int bytesSended = 0; //
    int n; //
    while (bytesSended < lenbuf)  {
        n = send(s, buf + bytesSended, lenbuf - bytesSended, flags);
        if (n < 0) {
            printf("Error with send in sendn\n");
            break;
        }
        bytesSended += n;
    }
    return (n == -1 ? -1 : bytesSended);
}

int recvLine(SOCKET sock, char* buffer, int buffSize) { //функция приема сообщения
    char* buff = buffer; //указатель на начало внешнего буфера
    char* currPosPointer; //указатель для работы со временным буфером
    int count = 0; //число прочитанных символов (без удаления из буфера сокета)
    char tempBuf[100]; //временный буфер для приема
    char currChar; //текущий анализируемый символ (ищем разделитель)
    int tmpcount = 0;
    while (--buffSize > 0){
        if (--count <= 0) {
            recvn(sock, tempBuf, tmpcount);
            count = recv(sock, tempBuf, sizeof(tempBuf), MSG_PEEK);
            if (count <= 0) { return count; }
            currPosPointer = tempBuf;
            tmpcount = count;
        }
        currChar = *currPosPointer++;
        *buffer++ = currChar;
        if (currChar == '\n') {
            *(buffer - 1) = '\0';
            recvn(sock, tempBuf, tmpcount - count + 1);
            return buffer - buff - 1;
        }
    }
    return -1;
}

int sendLine(int sock, char* str) {
    char tempBuf[MAX_STR_LEN];
    strcpy(tempBuf, str);
    if (tempBuf[strlen(tempBuf) - 1] != '\n')
        strcat(tempBuf, "\n");
    return sendn(sock, tempBuf, strlen(tempBuf), 0);
}

int main(void) { //используется для инициализации библиотеки сокетов
    WSADATA WSStartData; //Инициализация WinSock и проверка его запуска
    if (WSAStartup(MAKEWORD(2, 0), &WSStartData) != 0) {
        printf("WSAStartup failed with error: %ld\n", GetLastError());
        return 100;
    }
    int er_code=0; // инициализация клиентского сокета
    printf("Client is started.\nTry to create socket\n");
    int client_socket = socket( AF_INET, SOCK_STREAM, 0 );
    printf("socket created successfully\n");
    struct sockaddr_in sin;
    sin.sin_addr.s_addr=inet_addr("192.168.1.36");
    sin.sin_port=htons(7500);
    sin.sin_family=AF_INET; // установливаем TCP-соединение
    printf("try to connect to server\n");
    if(connect(client_socket, (struct sockaddr *) &sin,sizeof(sin))!=0) {
        printf("connect failed with error: %d\n", er_code);
        return SOCKET_ERROR;
    }
    printf("Client connected sucessfully\nEnter msg to send\n---------------------\n");
    char buf[SIZE_OF_BUF]; //буфер для приема и передачи сообщений
    strcpy(buf, "this is sparta");
    while(1) {
        sendLine(client_socket,buf);
        recvLine(client_socket,buf,SIZE_OF_BUF);
        printf("get msg from serv: \"%s\"\n**********************\n",buf);
        Sleep(20000);
    } // заканчиваем работу с сокетом клиента
    closesocket(client_socket);
    return 0;
}
