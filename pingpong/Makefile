CXX = g++
CXXFLAGS = $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs) -lSDL2_ttf

TARGET = ping_pong
SRC = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
