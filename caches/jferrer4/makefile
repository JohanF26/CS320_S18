all: Project2

Project2: Project2.o DirectMapped.o SetAssociative.o
					g++ Project2.o DirectMapped.o SetAssociative.o -o cache-sim

Project2.o: Project2.cpp
						g++ -c Project2.cpp

DirectMapped.o: DirectMapped.cpp
								g++ -c DirectMapped.cpp

SetAssociative.o: SetAssociative.cpp
									g++ -c SetAssociative.cpp

clean:
			rm -f *.o cache-sim
