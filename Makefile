objects = servermain.o client.o serverA.o graph.o
# serverB.o
exe = servermain client serverA graph

%.o: %.cpp
	g++ -std=c++11 -g -c -o $@ $^
	
all: $(objects)
	g++ -g -o servermain servermain.o
	g++ -g -o client client.o
	g++ -g -o serverA serverA.o
	g++ -g -o graph graph.o
	# g++ -g -o serverB serverB.o

.PHONY: clean
clean:
	rm $(objects) $(exe)
