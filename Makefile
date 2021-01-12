app: todo.o
	g++ todo.o -o todo.out

todo.o:
	g++ -c -std=c++11 -Wall todo.cpp -o todo.o

clean:
	rm -f todo.o todo.out