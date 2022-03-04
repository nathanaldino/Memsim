memsim: memsim.o
	g++ -o memsim memsim.o

memsim.o: memsim.cpp
	g++ -c memsim.cpp