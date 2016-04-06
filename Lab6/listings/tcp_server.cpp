#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#define MAX_STR_LEN 255
#define SIZE_OF_BUF 255


int recvn(SOCKET fd, char *bp, size_t len) {
    return recv(fd, bp, len, MSG_WAITALL);
}


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
            count = recv(sock, tempBuf, sizeof (tempBuf), MSG_PEEK);
            if (count <= 0) { return count; }
            currPosPointer = tempBuf;
            tmpcount = count;
        }
        currChar = *currPosPointer++;
        *buffer++ = currChar;
        if (currChar == '\n') {
            *(buffer - 1) = '\0';
            recvn(sock, tempBuf, tmpcount - count + 1);
            return buffer - buff-1;
        }
    }
    return -1;
}

int sendLine(int sock, char* str) {
    char tempBuf[MAX_STR_LEN];
    strcpy(tempBuf, str);
    if(tempBuf[strlen(tempBuf)-1]!='\n')
        strcat(tempBuf, "\n");
    return sendn(sock, tempBuf, strlen(tempBuf), 0);
}

DWORD WINAPI threadHandler(LPVOID param){
    SOCKET client_socket = (SOCKET)param;
    if (client_socket == INVALID_SOCKET) {
        printf("error with accept socket. GetLasterror= %ld\n", GetLastError());
        return 1003;
    }
    char buf[SIZE_OF_BUF]; //буфер приема и передачи сообщения
    int readbytes; //число прочитанных байт
    while (1) {
        if ((readbytes = recvLine(client_socket, buf, SIZE_OF_BUF)) == 0) {
            printf("Connection refused\n");
            break;
        }
        else if (readbytes == -1) {
            printf("buf is small\n");
            return 2000;
        }
        printf("get msg from client \"%s\" with size= %d\n", buf, readbytes);
        sendLine(client_socket, buf); //sendn(client_socket,buf,readbytes,0); //шлем сообщение обратно клиенту
        if (strncmp(buf, "exit", 4) == 0) break;
    }
    closesocket(client_socket);
    return 0;
}


int main(void) {
    //используется для инициализации библиотеки сокетов
    WSADATA WSStartData; //Инициализация WinSock и проверка его запуска
    if (WSAStartup(MAKEWORD(2, 0), &WSStartData) != 0) {
        printf("WSAStartup failed with error: %ld\n", GetLastError());
        return 100;
    } //создание сокета
    SOCKET server_socket; //по умолчанию используется протокол tcp
    printf("Server is started.\nTry to create socket -----------------");
    if((server_socket = socket( AF_INET, SOCK_STREAM, 0 )) ==INVALID_SOCKET) {
        printf("error with creation socket. GetLasterror= %ld\n",GetLastError());
        return 1000;
    }
    printf("CHECK\n"); //Привязывание сокета конкретному IP и номеру порта
    struct sockaddr_in sin; sin.sin_addr.s_addr=inet_addr("127.0.0.1"); // используем локальную машину
    sin.sin_port=htons(7500); // может быть любым кроме зарезервированных
    sin.sin_family=AF_INET; printf("Try to bind socket -------------------");
    if ( bind( server_socket, (struct sockaddr *)&sin, sizeof(sin) ) !=0 ) {
        printf("error with bind socket. GetLasterror= %ld\n",GetLastError());
        return 1001;
    }
    printf("CHECK\n"); //делаем сокет прослушиваемым
    printf("Try to set socket listening ----------");
    if(listen(server_socket,5 )!=0) {
        printf("error with listen socket. GetLasterror= %ld\n",GetLastError());
        return 1002;
    }
    printf("CHECK\n");
    printf("Server starts listening\n"); //Ждем клиента. Создаем пустую структуру, которая будет содержать параметры сокета, инициирующего соединение
    struct sockaddr_in from;
    int fromlen=sizeof(from); // начинаем "слушать" входящие запросы на подключение
    SOCKET client_socket=accept(server_socket,(struct sockaddr*)&from,&fromlen);
    if(client_socket==INVALID_SOCKET) {
        printf("error with accept socket. GetLasterror= %ld\n",GetLastError());
        return 1003;
    }
    printf("get client with IP= %s, port = %d\n",inet_ntoa(from.sin_addr),ntohs(from.sin_port));
    char buf[SIZE_OF_BUF]; //буфер приема и передачи сообщения
    int readbytes; //число прочитанных байт
    while(1) {
        if((readbytes=recvLine(client_socket,buf,SIZE_OF_BUF))==0) {
            printf("Connection refused\n");
            break;
        }
        else if(readbytes==-1) {
            printf("buf is small\n");
            return 2000;
        }
        printf("get msg from client \"%s\" with size= %d\n",buf,readbytes);
        sendLine(client_socket,buf); //sendn(client_socket,buf,readbytes,0); //шлем сообщение обратно клиенту
        if (strncmp(buf, "exit", 4) == 0) break;
    }
    closesocket(client_socket);
    closesocket(server_socket);
    return 0;
}
