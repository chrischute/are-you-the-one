# Makefile for Are You The One? simulator
# Christopher Chute

CC     = g++
CFLAGS = -std=c++11 -Wall
TARGET = ayto
DPNDS1 = Perms

all: $(TARGET)

$(TARGET): $(TARGET).o $(DPNDS1).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(DPNDS1).o

$(TARGET).o: $(TARGET).cpp $(DPNDS1).h
	$(CC) $(CFLAGS) -c $(TARGET).cpp

$(DPNDS1).o: $(DPNDS1).cpp $(DPNDS1).h
	$(CC) $(CFLAGS) -c $(DPNDS1).cpp

clean:
	$(RM) $(TARGET) *.exe *.exe.stackdump *.o *~
