# Makefile for Are You The One? simulator
# Christopher Chute

CC     = g++
CFLAGS = -std=c++11 -Wall
TARGET = ayto
DPNDS1 = Perms
DPNDS2 = PerfectMatching
DPNDS3 = Matches
DPNDS4 = Match

all:
	@echo "Error: Call 'make' from parent directory, above src."
	@echo "Nothing done."

$(TARGET): $(TARGET).o $(DPNDS1).o $(DPNDS2).o $(DPNDS3).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(DPNDS1).o $(DPNDS2).o $(DPNDS3).o

$(TARGET).o: $(TARGET).cpp $(TARGET).h $(DPNDS1).h $(DPNDS2).h $(DPNDS3).h $(DPNDS4).h
	$(CC) $(CFLAGS) -c $(TARGET).cpp

$(DPNDS1).o: $(DPNDS1).cpp $(DPNDS1).h $(DPNDS2).h $(DPNDS4).h
	$(CC) $(CFLAGS) -c $(DPNDS1).cpp

$(DPDNS2).o: $(DPNDS2).cpp $(DPNDS2).h
	$(CC) $(CFLAGS) -c $(DPNDS2).cpp

$(DPNDS3).o: $(DPNDS3).cpp $(DPNDS3).h $(DPNDS4).h
	$(CC) $(CFLAGS) -c $(DPNDS3).cpp

clean:
	$(RM) $(TARGET) *.exe *.exe.stackdump *.o *~
