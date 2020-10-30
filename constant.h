#ifndef CONSTANT_H
#define CONSTANT_H

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <netinet/in.h>

const std::string LOCAL_IP = "127.0.0.1";
const std::string REQUEST_COUNTRY_LIST = "COUNTRY_LIST_REQUEST";
const std::string NO_COUNTRY_FOUND = "does not show up in server A&B";
const std::string NO_USER_FOUND = "User not found";
const std::string NO_RECOMMENDATION = "No recommendation";
const std::string RECOMMENDATION = "Recommendation User";
const std::string ERROR = "Error in Message";



const int UDP_SERVER_A = 30051;
const int UDP_SERVER_B = 31051;
const int UDP_MAIN_SERVER = 32051;
const int TCP_MAIN = 33051;
const int BUFFER_LENGTH = 4096;


#endif 

