# Makefile for Are You The One? simulator
# Christopher Chute

CC     = g++
CFLAGS = -std=c++11 -Wall
TARGET = ayto
DPNDS1 = PerfectMatching
DPNDS2 = PerfectMatchingSet
DPNDS3 = TruthBooth
DPNDS4 = TruthBoothSet

all:
	@echo "Error: Call 'make' from parent directory, above src."
	@echo "Nothing done."

$(TARGET): $(TARGET).o $(DPNDS1).o $(DPNDS2).o $(DPNDS4).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(DPNDS1).o $(DPNDS2).o $(DPNDS4).o

$(TARGET).o: $(TARGET).cpp $(TARGET).h $(DPNDS2).h $(DPNDS1).h $(DPNDS4).h $(DPNDS3).h
	$(CC) $(CFLAGS) -c $(TARGET).cpp

$(DPNDS1).o: $(DPNDS1).cpp $(DPNDS1).h
	$(CC) $(CFLAGS) -c $(DPNDS1).cpp

$(DPNDS2).o: $(DPNDS2).cpp $(DPNDS2).h $(DPNDS1).h $(DPNDS3).h
	$(CC) $(CFLAGS) -c $(DPNDS2).cpp

$(DPNDS4).o: $(DPNDS4).cpp $(DPNDS4).h $(DPNDS3).h
	$(CC) $(CFLAGS) -c $(DPNDS4).cpp

clean:
	$(RM) $(TARGET) *.exe *.exe.stackdump *.o *~
