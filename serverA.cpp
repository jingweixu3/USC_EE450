#include "constant.h"
#include "udp.h"

using namespace std;

int main(){

    // create country list and build graph

    // coutry list, userlist in each country, user networking graph


    // initialize UDP server
    int sockfd;
    sockaddr_in serverAddr, clientAddr;
    char buffer[4096];
    socklen_t addr_size;

    create_UDP(sockfd, serverAddr, UDP_SERVER_A, LOCAL_IP);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr))){
        cout << "Error in binding.\n";
		return -1;
    }

    cout << "ServerA is up and running!" << endl;
    
    // receiving and send back
    while (1) {
        memset(&buffer, '\0', sizeof(buffer));
        memset(&clientAddr, '\0', sizeof(clientAddr));
        
        socklen_t  siaddr_size = sizeof(clientAddr);

        recvfrom(sockfd, buffer, 1024, 0, (sockaddr*) &clientAddr, &siaddr_size);
        cout << "Data Received:" << buffer << endl;

        sendto(sockfd, buffer, 1024, 0, (sockaddr*) &clientAddr, sizeof(clientAddr));
        cout << "Data Send: " << buffer << endl;
    }
    
    close(sockfd);

    return 0;
}