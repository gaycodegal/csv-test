SRC = ./src
OBJS = $(patsubst %.cc,%.o,$(wildcard $(SRC)/*.cc))
CC = g++
CPPFLAGS = -lcsv -I./src
OBJ_NAME = main

all: $(OBJS)
	$(CC) $(OBJS) $(CPPFLAGS) -o $(OBJ_NAME)
clean:
	rm -f $(OBJS) $(wildcard *~) $(wildcard $(SRC)/*~) $(wildcard $(INC)/*~)
fclean:
	rm -f $(OBJS) $(OBJ_NAME) $(wildcard *~)
format:
	clang-format -i $(find src/ -iname '*.cc')
	clang-format -i $(find src/ -iname '*.hh')

