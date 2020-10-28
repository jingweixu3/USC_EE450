objects = servermain.o client.o clientUDP.o serverA.o 
# serverB.o
exe = servermain client

%.o: %.cpp
	g++ -std=c++11 -g -c -o $@ $^
	
all: $(objects)
	g++ -g -o servermain servermain.o
	g++ -g -o  client client.o
	g++ -g -o  clientUDP clientUDP.o
	g++ -g -o serverA serverA.o
	# g++ -g -o serverB serverB.o

.PHONY: clean
clean:
	rm $(objects) $(exe)
