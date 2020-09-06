CC=g++
CFLAGS=-I. -Wall -Werror -Wextra #-std=c++11 -Wc++11-extensions
EXEC=avm

all:	main.o OpenRead.o OperandFactory.o DivByZero.o OverUnderFlow.o
	$(CC) -o $(EXEC) main.o OpenRead.o OperandFactory.o DivByZero.o OverUnderFlow.o $(CFLAGS)

clean:
	rm *.o

fclean: $(EXEC)
	rm $(EXEC)
	make clean

make re:
	make clean
	make
