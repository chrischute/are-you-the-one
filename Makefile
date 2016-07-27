# Makefile
# Christopher Chute

# Top-level makefile for Are You The One? simulator.
# Should be silent, let src/Makefile do all the printing.

SOURCE_DIR = src/
TARGET     = ayto

# Build from src, copy the executable up a level.
all:
	@cd $(SOURCE_DIR); make $(TARGET); cp $(TARGET) ../$(TARGET); make clean

# Remove all binaries and object files.
clean:
	@$(RM) $(TARGET) *.exe *.exe.stackdump *.o *~
	@cd $(SOURCE_DIR); make clean
