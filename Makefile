CXX=g++
CXXFLAGS=-Wall -Wextra -g -lm
OBJ=main.o API_handler.o routes.o
OBJ_PATH=./obj/
OBJS_WITH_PATH = $(foreach obj,$(OBJ),$(OBJ_PATH)$(obj))

build: optimal_route_gen

optimal_route_gen: $(OBJS_WITH_PATH)
	$(CXX) $(CXXFLAGS) $^ -o exec

$(OBJ_PATH)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

clean:
	rm -f $(OBJ_PATH)*.o exec