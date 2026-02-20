CXX = g++
# Added -Iinclude so the compiler looks in the include/ folder for .h files
CXXFLAGS = -std=c++17 -Wall -g -Iinclude
LDFLAGS = -lssl -lcrypto

TARGET = a

# Prefix your source files with the src/ directory
SRCS = src/main.cpp src/block.cpp src/blockchain.cpp src/trans.cpp

# This automatically transforms src/file.cpp into src/file.o
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# This pattern rule now handles files inside the src/ directory
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: all
	./$(TARGET)