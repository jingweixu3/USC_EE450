#include "constant.h"

using namespace std;

bool checkUserID(string userID);
bool checkCountryName(string countryName);

int main() {
    
    //	Create a socket
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock == -1) {
        return -1;
    }

    //	Create a structure for the server we're connecting with

    sockaddr_in server_addr, my_addr;
    server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(TCP_MAIN);
	server_addr.sin_addr.s_addr = inet_addr(LOCAL_IP.c_str());

    //	Connect to the server on the socket
    int connectRes = connect(client_sock, (sockaddr*)&server_addr, sizeof(server_addr));
    if (connectRes == -1) {
        return -1;
    }

    // get my port and IP
    int myPort;
    char myIP[16];
    bzero(&my_addr, sizeof(my_addr));
	socklen_t len = sizeof(my_addr);
	getsockname(client_sock, (struct sockaddr *) &my_addr, &len);
	inet_ntop(AF_INET, &my_addr.sin_addr, myIP, sizeof(myIP));
	myPort = ntohs(my_addr.sin_port);

    //	While loop:
    char buf[4096];
    string userInput;

    cout << "The client is up and running at port <" << myPort << ">" << endl <<endl;

    while (true) {

        cout<< endl << "----------------------------------------------------" << endl;
        cout << "[+] <" << myPort << "> Please enter the user ID: ";
    
        // input userID and check validation
        string userID, countryName;
        cin >> userID;

        if (!checkUserID(userID)) {
            continue;
        }

        // input country name and check validation
        cout << "[+] <" << myPort << "> Please enter the Country Name:";
        cin >> countryName;    // according to the requirements, country name does not contain white space, just use cin 
        
        if (!checkCountryName(countryName)) {
            continue;
        }

        userInput = userID + " " + countryName;  

        //  Send to server
        int sendRes = send(client_sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1) {
            cout << "[-] <" << myPort << "> Could not send to server! Whoops!\r\n";
            continue;
        }

        cout << endl <<"[+] <" << myPort << "> Client has sent User <" << userID << "> and <" << countryName << "> to Main Server using TCP." << endl << endl;

        //	Wait for response
        memset(buf, 0, 4096);
        
        int bytesReceived = recv(client_sock, buf, 4096, 0);
        if (bytesReceived == -1) {
            cout << "[-] <" << myPort << "> There was an error getting response from server" << endl << endl;
        }
        else {
            //	Display response
            cout << endl << "[+] <" << myPort << "> Receiving from TCP SERVER < "
                 << inet_ntoa(server_addr.sin_addr) << ": " << ntohs(server_addr.sin_port) << "> :" << string(buf, bytesReceived) << endl;
        }
    }

    //	Close the socket
    close(client_sock);

    return 0;
}

//  check whether the input is valid
bool checkUserID(string userID) {

    //  check userID
    for (char digit : userID) {
        if (!isdigit(digit)) {
            cout << "Invalid userID!, UserID has to be all digits!" << endl;
            return false;
        }
    }
    return true;
}

bool checkCountryName(string countryName) {
    //  check country name
    for (char letter : countryName) {
        if (!isalpha(letter)) {
            cout << "Invalid country name! Country Name has to be all alphabetic letter!" <<endl;
            return false;
        }
    }
    return true;
}