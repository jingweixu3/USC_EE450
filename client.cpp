
#include "constant.h"

using namespace std;


bool checkUserID(string userID, string countryName);
bool checkCountryName(string countryName);

int main() {
    
    //	Create a socket
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock == -1) {
        return -1;
    }

    //	Create a structure for the server we're connecting with

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TCP_MAIN);
    inet_pton(AF_INET, LOCAL_IP.c_str(), &server_addr.sin_addr);

    //	Connect to the server on the socket
    int connectRes = connect(client_sock, (sockaddr*)&server_addr, sizeof(server_addr));
    if (connectRes == -1) {
        return -1;
    }

    //	While loop:
    char buf[4096];
    string userInput;

    cout << "The client is up and running\n";

    while (true) {
        cout << "Please enter the user ID: ";
    

        // input userID and check validation
        string userID, countryName;
        cin >> userID;

        if (!checkUserID(userID)) {
            continue;
        }

        // input country name and check validation
        cout << "please enter the Country Name:";
        getline(cin, countryName);  
        
        if (!checkCountryName(countryName)) {
            continue;
        }

        userInput = userID + " " + countryName;  

        //  Send to server
        int sendRes = send(client_sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1) {
            cout << "Could not send to server! Whoops!\r\n";
            continue;
        }

        cout << "Client1 has sent User <" << userID << "> <" << countryName << "> to Main Server using TCP\n";

        //	Wait for response
        memset(buf, 0, 4096);
        
        int bytesReceived = recv(client_sock, buf, 4096, 0);
        if (bytesReceived == -1) {
            cout << "There was an error getting response from server\r\n";
        }
        else {
            //	Display response
            cout << "<SERVER> " << string(buf, bytesReceived) << "\r\n";
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
        if (!isalpha(letter) || letter != ' ') {
            cout << "Invalid country name! Country Name has to be all alphabetic letter!" <<endl;
            return false;
        }
    }
    return true;
}