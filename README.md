# USC-EE450-Socket-Programming
USC EE 450 project


a. Name: Jingwei Xu
   USC NetID: 

b. Have done in the assignment:
        
        In this project, I implemented an application to generate customized recommendatinos based on the
    user queries. We have multiple clients sending queries to the TCP server and the TCP server will reply 
    the user's queries based on the replies it hear back from the two backend UDP servers.
    
        For simplicity, we have two backend UDP servers: server A and server B, one main server and two clients. 
    Both server A and server B stores user infomation in differen countries. And according to the user infomation
    the UDP server will recommend the user when it receive the request from the main server. Main server will receive 
    the countries list from each UDP server once it booting up. It will receive query sending by TCP from clients and 
    according to the query, it will send request to UDP server and ask for recommendation. After it get the reply, it 
    will send the response back to client.

c. Port Number:
        Backend-Server A: UDP 30501
        Backend-Server B: UDP 31501
        Main Server: UDP 32501
                     TCP 33501
        Client: Dynamic TCP PORT

d. Code files:

        1. client.cpp
                (ask country name and user ID from terminal)
                - Create TCP socket with main server
                - Connect with main server
                - Send user ID and country name to main server
                - Hear back from main server

        2. mainserver.cpp
                (keep track of the whole process and print each step out)
                - Create UDP socket
                - Create TCP socket
                - Connect with Server A and Server B
                - Connect with client
                - Send and receive from Client
                - Send and receive from Server A and Server B
                - Check client query is valid or not

        3. serverA.cpp / serverB.cpp
                (storage countries and user infomation)
                - Read data file and store data
                - Create UDP socket
                - Connect with, send to and receive from main server 
                - Recommend new user based on the user ID 

        4. constant.h
                - Store all the default variables)
                
        5. country.h
                - Create country class to store the user ID as list and user's friend list
                - Create functions to build the countrr from the data file
                - Create recommendation system in this file
                
        6. tcp.h
                - Create TCP socket, connection for main server
                
        7. udp.h
                - Create UDP socket send, receive for main server


e. The format of all the messages exchanged
        
        All the messages are exchanged in the form of String
        
        [+] -- Good message
        [-] -- Error message
        [+] <port#> -- Current terminal port number
       
       1. Client:
                - User input data format: 
                    --- [+] <53634> Please enter the user ID: 432424
                    --- [+] <53634> Please enter the Country Name:ferf
                    --- [+] <53636> Please enter the user ID: 2313f
                                    Invalid userID!, UserID has to be all digits!
                    --- [+] <53636> Please enter the user ID: 2131
                        [+] <53636> Please enter the Country Name:dihi213
                        Invalid country name! Country Name has to be all alphabetic letter!
                        
                - Sending data example:
                    --- [+] <53634> Client has sent User <162118937> and <hSUMJxvw> to TCP Main Server <127.0.0.1: 33051>.

                - Receive data:
                    --- [+] <53634> Receiving from TCP SERVER < 127.0.0.1: 33051> : Recommendation User <313820140>
                    --- [+] <53636> Receiving from TCP SERVER < 127.0.0.1: 33051> : Country <China> does not show up in server A&B
                    --- [+] <53636> Receiving from TCP SERVER < 127.0.0.1: 33051> : <162118937> User not found

        2. Main Server 
                - Request country list via UDP
                    --- [+] <32051> Sending to UDP <127.0.0.1: 30051> : COUNTRY_LIST_REQUEST
                    --- [+] <32051> Receiving from UDP <127.0.0.1: 30051> : SbvedgUc BSYqz yUXpIilh BDLJzPUG Cab cCb fZlUg 
                    --- [+] <32051> Country list from server A <127.0.0.1: 30051> : SbvedgUc BSYqz yUXpIilh BDLJzPUG Cab cCb fZlUg 
                
                - Connection with TCP client
                    --- [+] Connection accepted from 127.0.0.1: 53634
                    --- [+] Connection accepted from 127.0.0.1: 53636
                    
                - Receiving from / Sending to client via TCP
                    --- [+] <33051> Request from Client: <127.0.0.1: 53634> : 162118937 hSUMJxvw
                    --- [+] <33051> Sending to Client <127.0.0.1: 53634> : Recommendation User <313820140>
                
                - Receiving from / Sending to server A, server B via UDP
                    --- [+] <32051> Sending to UDP <127.0.0.1: 30051> : 162118937 cCb
                    --- [+] <32051> Receiving from UDP <127.0.0.1: 30051> : <162118937> User not found
                    --- [+] <32051> Sending to UDP <127.0.0.1: 31051> : 162118937 hSUMJxvw
                    --- [+] <32051> Receiving from UDP <127.0.0.1: 31051> : Recommendation User <313820140>

        3. Server A / Server B
                - Boot up and show the country and user information: 
                    --- cYLEUu      98        4194
                        wUtyLVMQelbBIJuap      74        3534
                        hSUMJxvw      90        4650
                        
                - Receiving from/ Sending to main server via UDP
                    --- [+] <31051> Receiving from <127.0.0.1: 32051>: COUNTRY_LIST_REQUEST
                    --- [+] <31051> Sending to <127.0.0.1: 32051>: cYLEUu wUtyLVMQelbBIJuap hSUMJxvw     
                    --- [+] <30051> Receiving from <127.0.0.1: 32051>: 162118937 cCb
                    --- [+] <30051> Sending to <127.0.0.1: 32051>: <162118937> User not found
                    --- [+] <31051> Receiving from <127.0.0.1: 32051>: 162118937 hSUMJxvw
                    --- [+] <31051> Sending to <127.0.0.1: 32051>: Recommendation User <313820140>

g. Idiosyncrasy
    Based on the test results run on given environment (Ubuntu), there was no idiosyncrasy found

h. Refer code:
   
    1. Youtube link: https://www.youtube.com/watch?v=BIJGSQEipEE 
            Multiple client TCP server
            Create sockets (TCP / UDP);
            Bind a socket;
            Send & receive;
    
    2. Github link : https://gist.github.com/listnukira/4045436
            Get client own TCP IP address and Port number







