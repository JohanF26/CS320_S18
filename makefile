all:	Project1

Project1: Project1.o Predictors.o
					g++ Project1.o Predictors.o -o Project1

Project1.o: Project1.cpp
					g++ -c Project1.cpp

Predictors.o: Predictors.cpp
					g++ -c Predictors.cpp

clean:
					rm -f *.o Project1
