#include <winsock2.h>
#include <iostream>
#include <random>
#include <set>
#include <sstream>
#include <ctime>
#include <thread>

using namespace std;
// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
struct protokol {
    int iResult = 0;

    WSADATA wsaData;
    SOCKET SendSocket = INVALID_SOCKET;
    SOCKET RecvSocket;
    sockaddr_in RecvAddr;

    unsigned short Port = 27015;
    char SendBuf[1024];
    char RecvBuf[1024];
    int BufLen = 1024;

    sockaddr_in SenderAddr;
    int SenderAddrSize = sizeof(SenderAddr);
    std::string O;
    std::string o;
    std::string I;
    char czas[26];
    int liczba, odpowiedz;
    int L, L1, L2;
    int randomInt(int min, int max);
    char* setTime();
    std::string rzadanieID();
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
    static default_random_engine e{};
    uniform_int_distribution<int> d(min, max);
    return d(e);
}
int protokol::losowanie() {
    liczba = randomInt(L, L * 2);
    return liczba;
}
/*int protokol::setID() {
ID = randomInt(0, 50);
return ID;
}*/
char* protokol::setTime() {
    time_t result = time(nullptr);
    strftime(czas, sizeof(czas), "%c", std::localtime(&result));
    return czas;

}
std::string protokol::rzadanieID() {
    std::string s;
    O = "#ZadanieID#";
    s = "#O#$" + O + "#o#$#--#"+"#I#$#--#"+"#Czas#$#" + setTime() + "#";
    return s;
}
std::string protokol::przesylanieliczby() {
    std::string s;
    O = "#PrzesylanieLiczby#";
    std::cout << "Podaj liczba: ";
    std::cin >> L;
    losowanie();
    string czas = setTime();
    std::string x = std::to_string(L);
    s = "#O#$" + O + "#o#$#" + x + "##I#$#" + I + "##Czas#$#" + czas + "#";
    return s;
}
std::string protokol::przesylanieodpowiedzi() {
    std::string s;
    O = "#PrzsylanieOdopwiedzi#";
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
    s = "#O#$" + O  + "#o#$" + o + "#I#$#" + I + "##Czas#$#" + setTime() + "#";
    return s;
}
std::string protokol::nadawanieID() {
    std::string s;
    O = "#NadawanieID#";
    s = "#O#$" + O +  "#o#$" + "#--#" + "#I#$#" + I + "##Czas#$#" + setTime() + "#";
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
    s = "#O#$" + O  + "#o#$" + o + "#I#$#" + I + "##Czas#$#" + setTime() + "#";
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
    iResult = recvfrom(RecvSocket, RecvBuf, BufLen, 0, (SOCKADDR *)& SenderAddr, &SenderAddrSize);
    for (int i = 0; i < 250; i++) {
        s.push_back(RecvBuf[i]);
    }
    return s;
}
void protokol::nadawanie(std::string x) {
    int l = x.length();
    //char sendb[l];
    char* sendb = new char [l];
    for (int i = 0; i < x.length(); i++) {
        sendb[i] = x[i];
    }
    std::cout << x << std::endl;
    iResult = sendto(RecvSocket, sendb, l, 0, (SOCKADDR*)&SenderAddr, sizeof(SenderAddr));
}



class Klient{
public:
    int id;
   // thread t=thread(funkcja_serwerowa,std::ref(id));
Klient(int id){
    this->id=id;
       funkcja_serwerowa(id);
}

    void funkcja_serwerowa(int& id){
        protokol p;

        //-----------------------------------------------
        // Initialize Winsock
        p.iResult = WSAStartup(MAKEWORD(2, 2), &p.wsaData);
        if (p.iResult != NO_ERROR) {
            wprintf(L"WSAStartup failed with error %d\n", p.iResult);
        }
        //-----------------------------------------------
        // Create a receiver socket to receive datagrams
        p.RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (p.RecvSocket == INVALID_SOCKET) {
            wprintf(L"socket failed with error %d\n", WSAGetLastError());
        }

        //-----------------------------------------------
        // Bind the socket to any address and the specified port.
        p.RecvAddr.sin_family = AF_INET;
        p.RecvAddr.sin_port = htons(27015+id);
        p.RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        p.iResult = (bind(p.RecvSocket, (SOCKADDR *)& p.RecvAddr, sizeof(p.RecvAddr)));
        if (p.iResult == SOCKET_ERROR) {
            wprintf(L"bind failed with error %d\n", WSAGetLastError());
        }
        p.SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (p.SendSocket == INVALID_SOCKET) {
            wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
        }
        //-----------------------------------------------
        // Call the recvfrom function to receive datagrams
        // on the bound socket.
        while (true) {
            std::string s;
            s = "";
            s = p.odbieranie();
            std::cout << "Odebrane: " << s << std::endl;
            s = "";
            s = p.odbieranie();
            std::cout << "Odebrane: " << s << std::endl;
            p.nadawanie(p.potwierdzenie());
            std::string L = s.substr(39, 2);
            std::cout << "L:" << L << std::endl;
            p.L = stoi(L);
            p.nadawanie(p.przesylaniezakresu());
            s = "";
            s = p.odbieranie();
            std::cout << "Odebrane: " << s << std::endl;
            p.liczba = p.losowanie();
            std::string z;
            do {
                s = "";
                s = p.odbieranie();
                std::cout << "Odebrane: " << s << std::endl;
                z = "";
                z = s.substr(32, 2);
                std::cout << "Z: " << z << std::endl;
                p.odpowiedz = stoi(z);
                std::cout << "odp: " << p.odpowiedz << std::endl;
                p.sprawdzanie();
                p.nadawanie(p.sprawdzanieodpowiedzi());
            } while (p.o != "#DobraOdpowiedz#");
        }
        //-----------------------------------------------
        // Close the socket when finished receiving datagrams
        wprintf(L"Finished receiving. Closing socket.\n");
        p.iResult = closesocket(p.RecvSocket);
        if (p.iResult == SOCKET_ERROR) {
            wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
        }

        //-----------------------------------------------
        // Clean up and exit.
        wprintf(L"Exiting.\n");
        WSACleanup();
    }
};

void funkcja_serwerowa(int id){
    protokol p;
cout<<"5"<<endl;
    //-----------------------------------------------
    // Initialize Winsock
    p.iResult = WSAStartup(MAKEWORD(2, 2), &p.wsaData);
    if (p.iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error %d\n", p.iResult);
    }
    //-----------------------------------------------
    // Create a receiver socket to receive datagrams
    p.RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (p.RecvSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error %d\n", WSAGetLastError());
    }

    //-----------------------------------------------
    // Bind the socket to any address and the specified port.
    p.RecvAddr.sin_family = AF_INET;
    p.RecvAddr.sin_port = htons(27015+id);
    p.RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    p.iResult = (bind(p.RecvSocket, (SOCKADDR *)& p.RecvAddr, sizeof(p.RecvAddr)));
    if (p.iResult == SOCKET_ERROR) {
        wprintf(L"bind failed with error %d\n", WSAGetLastError());
    }
    p.SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (p.SendSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
    }
    //-----------------------------------------------
    // Call the recvfrom function to receive datagrams
    // on the bound socket.
    while (true) {
        stringstream ss;
        ss<<id;
        p.I=ss.str();//nadawanie ID
        std::string s;

        s = "";
        s = p.odbieranie();//odbieranie liczby
        std::cout << "Odebrane: " << s << std::endl;
        p.nadawanie(p.potwierdzenie());//potwierdzenie
        std::string L = s.substr(28, 2);
        std::cout << "L:" << L << std::endl;
        p.L = stoi(L);
        p.nadawanie(p.przesylaniezakresu());//przesłanie zakresu
        s = "";
        s = p.odbieranie();//odebranie potwierdzenia
        std::cout << "Odebrane: " << s << std::endl;
        p.liczba = p.losowanie();//losowanie
        std::string z;
        do {
            s = "";
            s = p.odbieranie();//odebranie odpowiedzi
            std::cout << "Odebrane: " << s << std::endl;
            p.nadawanie(p.potwierdzenie());//potwierdzenie
            z = "";
            z = s.substr(31, 2);
            std::cout << "Z: " << z << std::endl;
            p.odpowiedz = stoi(z);
            std::cout << "odp: " << p.odpowiedz << std::endl;
            p.sprawdzanie();
            p.nadawanie(p.sprawdzanieodpowiedzi());
            s = "";
            s = p.odbieranie();//odebranie potwierdzenia

        } while (p.o != "#DobraOdpowiedz#");
        cout<<"Zabawa skończone"<<endl;
    }
    //-----------------------------------------------
    // Close the socket when finished receiving datagrams
    wprintf(L"Finished receiving. Closing socket.\n");
    p.iResult = closesocket(p.RecvSocket);
    if (p.iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
    }

    //-----------------------------------------------
    // Clean up and exit.
    wprintf(L"Exiting.\n");
    WSACleanup();
}

int main()
{
    protokol msg;
    msg.iResult = WSAStartup(MAKEWORD(2, 2), &msg.wsaData);
    if (msg.iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error %d\n", msg.iResult);
        return 1;
    }
    //-----------------------------------------------
    // Create a receiver socket to receive datagrams
    msg.RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (msg.RecvSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error %d\n", WSAGetLastError());
        return 1;
    }

    //-----------------------------------------------
    // Bind the socket to any address and the specified port.
    msg.RecvAddr.sin_family = AF_INET;
    msg.RecvAddr.sin_port = htons(27015);
    msg.RecvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    msg.iResult = (bind(msg.RecvSocket, (SOCKADDR *)& msg.RecvAddr, sizeof(msg.RecvAddr)));
    if (msg.iResult == SOCKET_ERROR) {
        wprintf(L"bind failed with error %d\n", WSAGetLastError());
        return 1;
    }
    //-------------------------------------------------
    std::string s;
    vector<thread> klients;
    int i=1;

    //odbiera informację od klienta o rozpoczęciu przesyłu danych

    while(true){
        s = msg.odbieranie();//odbieranie zadanie ID
        std::cout<<"Odebrane: "<<s<<std::endl;
        std::string x = s.substr(5,1);//sprawdzanie czy wiadomosć jest poprawan
        std::cout<<"Pobrane: "<<x<<endl;
        if (x=="Z") {
            s.clear();
            stringstream ss;
            ss<<i;
            msg.I=ss.str();//nadaewanie ID
            std::cout<<"ID: "<<msg.I<<std::endl;
            msg.nadawanie(msg.potwierdzenie());//wysyła potwierdzenie
            //identyfikator sesji
            i++;
            x = msg.nadawanieID();//nadaje ID
            msg.nadawanie(x);
            s = "";
            s = msg.odbieranie();//odbieranie liczby
            std::cout << "Odebrane: " << s << std::endl;
            klients.push_back(thread(funkcja_serwerowa,stoi(msg.I)));//dodanie wątku

        }
    }
    return 0;
}