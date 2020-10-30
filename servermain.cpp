#include "constant.h"
#include "tcp.h"
#include "udp.h"
#include "country.h"

using namespace std;

int main(){

	// initialize udp
    int sockfd_UDP;
    sockaddr_in main_serverAddr_UDP, A_serverAddr_UDP, B_serverAddr_UDP;
    create_UDP(sockfd_UDP, main_serverAddr_UDP, UDP_MAIN_SERVER, LOCAL_IP);

    if (bind(sockfd_UDP, (sockaddr *) &main_serverAddr_UDP, sizeof(main_serverAddr_UDP)) < 0) {
        cout <<"[-] Error in binding in UDP A" << endl;
        return -1;
    }

    memset(&A_serverAddr_UDP, 0, sizeof(A_serverAddr_UDP));
    A_serverAddr_UDP.sin_family = AF_INET;
    A_serverAddr_UDP.sin_addr.s_addr = inet_addr(LOCAL_IP.c_str());
    A_serverAddr_UDP.sin_port = htons(UDP_SERVER_A);


    memset(&B_serverAddr_UDP, 0, sizeof(B_serverAddr_UDP));
    B_serverAddr_UDP.sin_family = AF_INET;
    B_serverAddr_UDP.sin_addr.s_addr = inet_addr(LOCAL_IP.c_str());
    B_serverAddr_UDP.sin_port = htons(UDP_SERVER_B);



    cout << "[+] Main Server is up and running" << endl << endl;

	// send country request 
		// from server A
	string countryList_A = UDP_send_receive(sockfd_UDP, A_serverAddr_UDP, REQUEST_COUNTRY_LIST);
	cout << "[+] <" << UDP_MAIN_SERVER <<"> Country list from server A <" << inet_ntoa(A_serverAddr_UDP.sin_addr) << ": " << ntohs(A_serverAddr_UDP.sin_port) 	
		 << "> : " << countryList_A << endl << endl;
	unordered_set<string> country_set_A = convert_string_to_set(countryList_A);
		// from server B
	string countryList_B = UDP_send_receive(sockfd_UDP, B_serverAddr_UDP, REQUEST_COUNTRY_LIST);
	cout << "[+] <" << UDP_MAIN_SERVER <<"> Country list from server B <" << inet_ntoa(B_serverAddr_UDP.sin_addr) << ": " << ntohs(B_serverAddr_UDP.sin_port) 	
		 << ">: "  << countryList_B << endl << endl;
	unordered_set<string> country_set_B = convert_string_to_set(countryList_B);
	
	// initialize tcp server
	int server_socket, bind_return;
	sockaddr_in serverAddr;

	if (create_TCP_server(server_socket, serverAddr, TCP_MAIN, LOCAL_IP) == false) {
		return -1;
	}

    int newSocket;

	while (1) {
		sockaddr_in newAddr; 

		socklen_t addr_size = sizeof(newAddr);
		pid_t childpid;

		//accept request
		newSocket = accept(server_socket, (sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0) {
			return -1;
		}
		cout << "[+] Connection accepted from " << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) << "\n";

		// child process 
		if((childpid = fork()) == 0) {
			close(server_socket);

			while (1) {

				// TCP receive from client
				char buffer[BUFFER_LENGTH];
    			memset(&buffer, '\0', sizeof(buffer));

				if (recv(newSocket, buffer, BUFFER_LENGTH, 0) == 0){
					continue;
				}
				cout<< endl << "----------------------------------------------------" << endl;

				cout << "[+] <" << TCP_MAIN << "> Request from Client: <" << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) 
					 << "> : "<< buffer << endl << endl;

				// split message
				vector<string> message_list = convert_string_to_vector(string(buffer));
				if (message_list.size() != 2) {
					cout << "[-] <" << TCP_MAIN << "> Error in message from client <" << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) 
						 << "> : "<< endl << endl;
					send(newSocket, ERROR.c_str(), ERROR.size() + 1, 0);

					continue;
				}

				int userID = stoi(message_list[0]);
				string country_name = message_list[1];
				string reply;

				// check country existed or not, and do something
				if (country_set_A.find(country_name) == country_set_A.end() && country_set_B.find(country_name) == country_set_B.end()) {
					
					// send back to client
					reply = "Country <" + country_name + "> " + NO_COUNTRY_FOUND;
					send(newSocket, reply.c_str(),reply.size() + 1, 0);
					cout << "[+] <" << TCP_MAIN << "> Sending to Client: <" << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) 
						 <<"> :" <<reply << endl << endl;
					continue;
				}
				else if (country_set_A.find(country_name) != country_set_A.end()) {
					reply = UDP_send_receive(sockfd_UDP, A_serverAddr_UDP, buffer);
				}
				else {
					reply = UDP_send_receive(sockfd_UDP, B_serverAddr_UDP, buffer);
				}

				// send back to client
				send(newSocket, reply.c_str(),reply.size() + 1, 0);
				cout << "[+] <" << TCP_MAIN << "> Sending to Client <" << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) 
					 << "> : " <<  reply << endl << endl;

			}
		}

	}
	close(newSocket);
	close(sockfd_UDP);

	return 0;
}
