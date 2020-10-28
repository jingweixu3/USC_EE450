#include "constant.h"
#include "tcp.h"
#include "udp.h"

using namespace std;

int main(){

    cout << "Main Server is up and running\n";

	// initialize udp
    int sockfd_UDP;
    sockaddr_in A_serverAddr_UDP;

    create_UDP(sockfd_UDP, A_serverAddr_UDP, UDP_SERVER_A, LOCAL_IP);

	// send country request 
	string countryList = UDP_send_receive(sockfd_UDP, A_serverAddr_UDP, REQUEST_COUNTRY_LIST);
	
	// initialize tcp server
	int server_socket, bind_return;
	sockaddr_in serverAddr;

	if (create_TCP(server_socket, serverAddr, TCP_MAIN, LOCAL_IP) == false) {
		return -1;
	}

	while(1) {
		int newSocket;
		sockaddr_in newAddr; 

		socklen_t addr_size;
		pid_t childpid;

		//accept request
		newSocket = accept(server_socket, (sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0) {
			return -1;
		}
		cout << "Connection accepted from " << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) << "\n";

		// child process 
		if((childpid = fork()) == 0) {
			close(server_socket);

			while(1) {

				// TCP receive from client
				char buffer[BUFFER_LENGTH];
    			memset(&buffer, '\0', sizeof(buffer));

				if (recv(newSocket, buffer, BUFFER_LENGTH, 0) == 0){
					continue;
				}
				
				cout << "Data from Client: " << buffer << endl;

				// check country existed or not, and do something

				// sending info to UDP
				
				string countryName = "China";

				string reply = UDP_send_receive(sockfd_UDP, A_serverAddr_UDP, countryName);

				// send back to client
				send(newSocket, reply.c_str(),reply.size() + 1, 0);
				cout << "Data send to Client: " <<  reply << endl;

			}
		}
		close(newSocket);

	}

	close(sockfd_UDP);
	close(server_socket);

	return 0;
}