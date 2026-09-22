CXX = g++
CXXFLAGS = -fdiagnostics-color=always -g
LDLIBS = -lsqlite3 -lncurses
TARGET = out/main
DB = *.db


build:
	$(CXX) $(CXXFLAGS) src/*.cpp $(LDLIBS) -o $(TARGET)

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET) $(DB)

