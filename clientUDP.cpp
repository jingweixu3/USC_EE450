#include "constant.h"

using namespace std;
int main(){

    int sockfd_UDP;
    sockaddr_in serverAddr_UDP;
    char buffer[4096];

    sockfd_UDP = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&serverAddr_UDP, '\0', sizeof(serverAddr_UDP));

    serverAddr_UDP.sin_family = AF_INET;
    serverAddr_UDP.sin_port = htons(UDP_SERVER_A);
    serverAddr_UDP.sin_addr.s_addr = inet_addr(LOCAL_IP.c_str());


    while (true) {
        string userInput;
        cout << "please input the message: " << endl;
        getline(cin, userInput);

        // strcpy(char_array, userInput.c_str())
        
        sendto(sockfd_UDP, userInput.c_str(), userInput.size() + 1, 0, (struct sockaddr*)&serverAddr_UDP, sizeof(serverAddr_UDP));

        socklen_t  serverAddr_UDP_length = sizeof(serverAddr_UDP);

        recvfrom(sockfd_UDP, buffer, 1024, 0, (sockaddr*) &serverAddr_UDP, &serverAddr_UDP_length);
        cout << "[+]Receiving: " << buffer << endl;

    }
   
    close(sockfd_UDP);
  return 0;

}