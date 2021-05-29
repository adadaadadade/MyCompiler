CC=g++
EXE=mycompiler
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
CXXFLAGS=-g -Wno-deprecated	#传入参数DEBUG
all:depend $(EXE) cleano
depend:
	@$(CC) -MM $(SRC) > .depend
-include .depend
$(EXE):$(OBJ)
	$(CC) $(OBJ) -o $(EXE) 
clean:
	@rm $(EXE) $(OBJ) .depend -f
cleano:
	@rm *.o

