#include "constant.h"
#include <stdio.h> 
#include <stdlib.h> 

using namespace std;

int main(){

    cout << "Main Server is up and running\n";

	// udp

    int sockfd_UDP;
    sockaddr_in A_serverAddr_UDP;
    char buffer[4096];

    sockfd_UDP = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&A_serverAddr_UDP, '\0', sizeof(A_serverAddr_UDP));

    A_serverAddr_UDP.sin_family = AF_INET;
    A_serverAddr_UDP.sin_port = htons(UDP_SERVER_A);
    A_serverAddr_UDP.sin_addr.s_addr = inet_addr(LOCAL_IP.c_str());

	// send country request 

	sendto(sockfd_UDP, REQUEST_COUNTRY_LIST.c_str(), REQUEST_COUNTRY_LIST.size() + 1, 0, (struct sockaddr*)&A_serverAddr_UDP, sizeof(A_serverAddr_UDP));

    socklen_t  A_serverAddr_UDP_length = sizeof(A_serverAddr_UDP);

	recvfrom(sockfd_UDP, buffer, 1024, 0, (sockaddr*) &A_serverAddr_UDP, &A_serverAddr_UDP_length);
	cout << "[+]Receiving: " << buffer << endl;

	// tcp 
	int server_socket, bind_return;
	sockaddr_in serverAddr;

	int newSocket;
	sockaddr_in newAddr; 

	socklen_t addr_size;
	pid_t childpid;

    // create server socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        cout << "Failed to create server socket.\n";
		return -1;
	}

    // Bind the ip address and port to a socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(TCP_MAIN);
    inet_pton(AF_INET, "0.0.0.0", &serverAddr.sin_addr);

	bind_return = bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	
    if(bind_return < 0) {
		cout << "Error in binding.\n";
		return -1;
	}

	if(listen(server_socket, 10) != 0) {
		cout << "Error in listening.\n";
        return -1;
	}

    memset(&buffer, '\0', sizeof(buffer));

	while(1) {
		newSocket = accept(server_socket, (sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0) {
			return -1;
		}
		cout << "Connection accepted from " << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) << "\n";

		// child process 
		if((childpid = fork()) == 0) {
			close(server_socket);

			while(1) {
				if (recv(newSocket, buffer, 4096, 0) == 0){
					continue;
				}
				
				cout << "Data from Client: " << buffer << endl;

				// check country existed or not, and do something

				// sending info to UDP
				
				
				sendto(sockfd_UDP, buffer, 4096, 0, (struct sockaddr*)&A_serverAddr_UDP, sizeof(A_serverAddr_UDP));
				cout << "Data send to UDP Server: " <<  buffer << endl;				
				recvfrom(sockfd_UDP, buffer, 4096, 0, (sockaddr*) &A_serverAddr_UDP, &A_serverAddr_UDP_length);
				cout << "Data from UDP Server: " <<  buffer << endl;

				// send back to client
				send(newSocket, buffer, strlen(buffer), 0);
				cout << "Data send to Client: " <<  buffer << endl;

				bzero(buffer, sizeof(buffer));	
			}
		}
	}

	close(sockfd_UDP);
	close(newSocket);
	close(server_socket);

	return 0;
}