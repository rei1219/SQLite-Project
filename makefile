CXX = g++
CXXFLAGS = -fdiagnostics-color=always -g
LDLIBS = -lsqlite3

TARGET = out/main

build:
	$(CXX) $(CXXFLAGS) src/*.cpp $(LDLIBS) -o $(TARGET)

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET)

