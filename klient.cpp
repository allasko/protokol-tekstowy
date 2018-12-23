#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <string>
#include <time.h>
#include <ctime>
#include <random>
//#include "protokol.h"

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

struct protokol {
    int iResult;
    WSADATA wsaData;

    SOCKET SendSocket = INVALID_SOCKET;
    SOCKET RecvSocket;
    sockaddr_in RecvAddr;

    unsigned short Port = 27015;

    char* SendBuf;
    char RecvBuf[1024];
    int BufLen = 1024;
    sockaddr_in SenderAddr;
    int SenderAddrSize = sizeof(SenderAddr);
    int RecvAddrSize = sizeof(RecvAddr);
    std::string O;
    std::string o;
    std::string I;
    char czas[26];
    int liczba, odpowiedz;
    int L, L1, L2;
    int randomInt(int min, int max);
    char* setTime();
    std::string zadanieID();
    int losowanie();
    bool sprawdzanie();
    std::string przesylanieliczby();
    std::string przesylanieodpowiedzi();
    std::string potwierdzenie();
    std::string nadawanieID();
    std::string przesylaniezakresu();
    std::string sprawdzanieodpowiedzi();
    std::string odbieranie();
    void nadawanie(std::string x);

};
int protokol::randomInt(int min, int max) {
    static std::default_random_engine e{};
    std::uniform_int_distribution<int> d(min, max);
    return d(e);
}
int protokol::losowanie() {
    liczba = randomInt(L, L * 2);
    return liczba;
}
char* protokol::setTime() {
    time_t result = time(nullptr);
    strftime(czas, sizeof(czas), "%c", std::localtime(&result));
    return czas;

}
std::string protokol::zadanieID() {
    std::string s;
    O = "#ZadanieID#";
    s = "#O#$" + O + "#o#$#--#" + "#I#$#--#" + "#Czas#$#" + setTime() + "#";
    return s;
}
std::string protokol::przesylanieliczby() {
    std::string s;
    O = "#PrzesylanieLiczby#";
    std::cout << "Podaj liczba: ";
    std::cin >> L;
    losowanie();
    std::string czas = setTime();
    std::string x = std::to_string(L);
    s = "#O#$" + O + "#o#$#" + x  +"##I#$#" + I + "##Czas#$#" + czas + "#";
    return s;
}
std::string protokol::przesylanieodpowiedzi() {
    std::string s;
    O = "#PrzsyłanieOdopwiedzi#";
    std::cout << "Podaj odpowiedz: ";
    std::cin >> odpowiedz;
    std::string x = std::to_string(odpowiedz);
    s = "#O#$" + O + "#o#$#" + x + "##I#$#" + I + "##Czas#$#" + setTime() + "#";
    return s;
}
std::string protokol::potwierdzenie() {
    std::string s;
    O = "#CzyDotarlo#";
    o = "#Dotarlo#";
    s = "#O#$" + O + "#o#$" + o + "#I#$#" + I + "##Czas#$#" + setTime() + "#";
    return s;
}
std::string protokol::nadawanieID() {
    std::string s;
    O = "#NadawanieID#";
    s = "#O#$" + O + "#o#$" + "#--#" + "#I#$#" + I + "##Czas#$#" + setTime() + "#";
    return s;
}
std::string protokol::przesylaniezakresu() {
    std::string s;
    O = "#PrzesylanieZakresu#";
    std::string x = std::to_string(L * 1);
    std::string y = std::to_string(L * 2);
    s = "#O#$" + O + "#o#" + "$#" + x + "," + y + "##I#$#" + I + "##Czas#$#" + setTime() + "#";
    return s;
}
std::string protokol::sprawdzanieodpowiedzi() {
    std::string s;
    O = "#SprawdzanieOdpowiedzi#";

    if (sprawdzanie()) {
        o = "#DobraOdpowiedz#";
    }
    else o = "#ZlaOdpowiedz#";
    s = "#O#$" + O + "#o#$" + o + "#I#$#" + I + "##Czas#$#" + setTime() + "#";
    return s;
}

bool protokol::sprawdzanie() {
    if (liczba == odpowiedz) {
        return true;
    }
    else return false;
}
std::string protokol::odbieranie() {
    std::string s;
    iResult = recvfrom(SendSocket, RecvBuf, BufLen, 0, (SOCKADDR *)& RecvAddr, &RecvAddrSize);
    //  std::cout<<"recvbuf: "<<RecvBuf;
    for (int i = 0; i < 250; i++) {
        s.push_back(RecvBuf[i]);
    }
    return s;
}
void protokol::nadawanie(std::string x) {
    int l = x.length();
    char* sendb = new char[l];
    for (int i = 0; i < x.length(); i++) {
        sendb[i] = x[i];
    }
    std::cout << x << std::endl;
    iResult = sendto(SendSocket, sendb, l, 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
}
int main()
{

    protokol p;
    int n;
    std::cin >> n;

    //----------------------
    // Initialize Winsock
    p.iResult = WSAStartup(MAKEWORD(2, 2), &p.wsaData);
    if (p.iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %d\n", p.iResult);
        return 1;
    }

    //---------------------------------------------
    // Create a socket for sending data
    p.SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (p.SendSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //---------------------------------------------
    // Set up the RecvAddr structure with the IP address of
    // the receiver (in this example case "127.0.0.1")
    // and the specified port number.
    p.RecvAddr.sin_family = AF_INET;
    p.RecvAddr.sin_port = htons(27015);
    p.RecvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    p.RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (p.RecvSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error %d\n", WSAGetLastError());
        return 1;
    }

    //---------------------------------------------
    // Send a datagram to the receiver
    std::string h;
    p.nadawanie(p.zadanieID());//wysyła rzadanie ID
    std::string x = "";
    x = p.odbieranie();//odbiera potwierdzenie
    std::cout << "Odebrane: " << x << std::endl;//wyswietl
    std::string s = "";
    s = p.odbieranie();//odbiera ID
    std::cout << "Odebrane: " << s << std::endl;//wyswietl
    h = s.substr(30, 1);//pobieranie numeru ID
    std::cout << "ID: " << h << std::endl;
    p.RecvAddr.sin_port = htons(27015+stoi(h));//Zmiana portu
    p.I = h;//przypisanie ID do zmieniej w structurze
    p.nadawanie(p.potwierdzenie());//nadanie potwierdzenia
    p.nadawanie(p.przesylanieliczby());//przesyłanie liczby z której tworzymy zakres
    s = "";//zerowanie stringa na wszelki wypadek...
    s = p.odbieranie();//odebranie potwierdzenia
    std::cout << "Odebrane: " << s << std::endl;//wyswietl
    s = "";//znowu zerowanie
    s = p.odbieranie();//odebranie zakresu L i L*2
    std::cout << "Odebrane: " << s << std::endl;//wyswietl
    p.nadawanie(p.potwierdzenie());//nadanie potwierdzenia
    std::string y;
    do {
        p.nadawanie(p.przesylanieodpowiedzi());//przesylanie odpowiedzi
        s = "";
        s = p.odbieranie();//odbieranie potwierdzenia
        std::cout << "Odebrane: " << s << std::endl;
        s = "";
        s = p.odbieranie();//odbieranie dobra czy zla odpowiedz
        std::cout << "Odebrane: " << s << std::endl;
        y = s.substr(31, 16);
        p.nadawanie(p.potwierdzenie());//nadanie potwierdzenia
        std::cout << "y: " << y << std::endl;
    } while (y != "#DobraOdpowiedz#");
    //---------------------------------------------
    // When the application is finished sending, close the socket.
    wprintf(L"Finished sending. Closing socket.\n");
    p.iResult = closesocket(p.SendSocket);
    if (p.iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //---------------------------------------------
    // Clean up and quit.
    wprintf(L"Exiting.\n");
    WSACleanup();
    return 0;
}