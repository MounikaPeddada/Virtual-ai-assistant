CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -Wall -Iinclude
CFLAGS = -O2 -Iinclude

# SQLite is compiled straight from the bundled amalgamation (sqlite3.c),
# so nobody needs libsqlite3-dev / apt / brew installed to build this.
ifeq ($(OS),Windows_NT)
    LDFLAGS =
else
    LDFLAGS = -ldl -lpthread
endif

SRC = $(wildcard src/*.cpp)
SQLITE_OBJ = include/sqlite3.o
TARGET = assistant

$(TARGET): $(SRC) $(SQLITE_OBJ)
	$(CXX) $(CXXFLAGS) $(SRC) $(SQLITE_OBJ) -o $(TARGET) $(LDFLAGS)

$(SQLITE_OBJ): include/sqlite3.c
	$(CC) $(CFLAGS) -c include/sqlite3.c -o $(SQLITE_OBJ)

run: $(TARGET)
	./$(TARGET)

ifeq ($(OS),Windows_NT)
clean:
	del /Q $(TARGET).exe $(SQLITE_OBJ) assistant.db notes.txt 2>nul || exit 0
else
clean:
	rm -f $(TARGET) $(SQLITE_OBJ) assistant.db notes.txt
endif

.PHONY: run clean