CXX = g++
CXXFLAGS = $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs)

TARGET = ping_pong
SRC = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
