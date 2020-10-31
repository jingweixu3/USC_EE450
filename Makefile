objects = servermain.o client.o serverA.o serverB.o
# serverB.o
exe = servermain client serverA serverB

%.o: %.cpp
	g++ -std=c++11 -g -c -o $@ $^

all: $(objects)
	g++ -std=c++11 -g -o servermain servermain.o
	g++ -std=c++11 -g -o client client.o
	g++ -std=c++11 -g -o serverA serverA.o
	g++ -std=c++11 -g -o serverB serverB.o

client: client.o
	g++ -std=c++11 -g -o client client.o

mainserver: servermian.o 
	g++ -std=c++11 -g -o servermain servermain.o

serverA: serverA.o
	g++ -std=c++11 -g -o serverA serverA.o

serverB: serverB.o
	g++ -std=c++11 -g -o serverB serverB.o


.PHONY: clean
clean:
	rm $(objects) $(exe)
