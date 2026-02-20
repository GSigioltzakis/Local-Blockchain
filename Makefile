CXX = g++

CXXFLAGS = -std=c++17 -Wall -g

LDFLAGS = -lssl -lcrypto

TARGET = a

SRCS = main.cpp block.cpp blockchain.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: all
	./$(TARGET)