OBJS = *.cpp
CXX = g++
DEPS = object.h
LFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
CXXFLAGS = -w -std=c++11 $(LFLAGS)

NAME = pong

all: $(OBJS) $(DEPS)
	$(CXX) $(OBJS) $(*.o) -o $(NAME) $(CXXFLAGS)
	
clean:
	rm -f *.o *.gch
	
play:
	make && make clean && ./$(NAME)
