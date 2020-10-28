#include "constant.h"


using namespace std;


int main(){

    int sockfd;

    sockaddr_in serverAddr, clientAddr;

    char buffer[1024];
    socklen_t addr_size;


    // create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // bind IP and port to the socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(UDP_SERVER_A);
    serverAddr.sin_addr.s_addr = inet_addr(LOCAL_IP.c_str());

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr))){
        cout << "Error in binding.\n";
		return -1;
    }

    // receiving and send back
    while (1) {
        memset(&buffer, '\0', sizeof(buffer));
        
        memset(&clientAddr, '\0', sizeof(clientAddr));
        
        socklen_t  siaddr_size = sizeof(clientAddr);

        recvfrom(sockfd, buffer, 1024, 0, (sockaddr*) &clientAddr, &siaddr_size);
        cout << "Data Received:" << buffer << endl;

        sendto(sockfd, buffer, 1024, 0, (sockaddr*) &clientAddr, sizeof(clientAddr));

    }
    
    close(sockfd);

    return 0;
}