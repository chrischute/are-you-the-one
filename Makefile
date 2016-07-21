# Makefile for Are You The One? simulator
# Christopher Chute

CC     = g++
CFLAGS = -std=c++11 -Wall -lpthread
TARGET = ayto

all: $(TARGET)

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o

$(TARGET).o: $(TARGET).cpp $(TARGET).h
	$(CC) $(CFLAGS) -c $(TARGET).cpp

clean:
	$(RM) $(TARGET) *.o *~
