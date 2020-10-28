#include "constant.h"

using namespace std;
int main(){



    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[4096];
    socklen_t addr_size;

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&serverAddr, '\0', sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(UDP_SERVER_A);
    serverAddr.sin_addr.s_addr = inet_addr(LOCAL_IP.c_str());


    while (true) {
        string userInput;
        cout << "please input the message: " << endl;
        getline(cin, userInput);

        // strcpy(char_array, userInput.c_str())
        
        sendto(sockfd, userInput.c_str(), userInput.size() + 1, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        socklen_t  siaddr_size = sizeof(serverAddr);

        recvfrom(sockfd, buffer, 1024, 0, (sockaddr*) &serverAddr, &siaddr_size);
        cout << "[+]Receiving: " << buffer << endl;

    }
   
    close(sockfd);
  return 0;

}