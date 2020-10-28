#include "constant.h"

using namespace std;
int main(){



    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    socklen_t addr_size;

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&serverAddr, '\0', sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(UDP_SERVER_A);
    serverAddr.sin_addr.s_addr = inet_addr(LOCAL_IP.c_str());


    
    strcpy(buffer, "Hello Server\n");
    sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("[+]Data Send: %s", buffer);

    socklen_t  siaddr_size = sizeof(serverAddr);

    recvfrom(sockfd, buffer, 1024, 0, (sockaddr*) &serverAddr, &siaddr_size);
    cout << "receiving: " <<buffer << endl;

  
  return 0;

}