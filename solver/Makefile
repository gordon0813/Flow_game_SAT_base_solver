CFLAGS = -O3 -Wall
CFLAGS = -g -Wall

Board: clean File.o Proof.o Solver.o Board.o Node.o Edge.o SATnumber.o
	g++ -o $@ $(CFLAGS) File.o Proof.o Solver.o Board.o Node.o Edge.o SATnumber.o

File.o: File.cpp
	g++ -c $(CFLAGS) File.cpp

Proof.o: Proof.cpp
	g++ -c $(CFLAGS) Proof.cpp

Solve.o: Solver.cpp
	g++ -c $(CFLAGS) Solver.cpp

Board.o: Board.cpp 
	g++ -c $(CFLAGS) Board.cpp
Node.o: Node.cpp
	g++ -c $(CFLAGS) Node.cpp
Edge.o: Edge.cpp
	g++ -c $(CFLAGS) Edge.cpp
SATnumber.o: SATnumber.cpp
	g++ -c $(CFLAGS) SATnumber.cpp


clean:
	rm -f *.o seatAss core* tags
