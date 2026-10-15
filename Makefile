# Makefile - COSC 369 Project 1
#
#   make        compile, build the static library, link, run, make Word/Excel
#   make clean  delete everything that was generated

# Macros
CC     = gcc
CFLAGS = -Wall -g
OBJS   = parser.o scheduler.o output.o
LIB    = libschedule.a
PROG   = schedule

# Default target
all: $(PROG) run docs

# Link main.o with the static library.
# -L. = look for libraries in this folder, -lschedule = use libschedule.a
$(PROG): main.o $(LIB)
	$(CC) $(CFLAGS) main.o -L. -lschedule -o $(PROG)

# Build the static library from the object files.
# ar = archive tool: r = add files, c = create, s = add an index
$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

# Compile each .c file into a .o file (-c = compile only, don't link)
main.o: main.c schedule.h parser.h scheduler.h output.h
	$(CC) $(CFLAGS) -c main.c

parser.o: parser.c parser.h schedule.h
	$(CC) $(CFLAGS) -c parser.c

scheduler.o: scheduler.c scheduler.h schedule.h
	$(CC) $(CFLAGS) -c scheduler.c

output.o: output.c output.h schedule.h
	$(CC) $(CFLAGS) -c output.c

# Run the program on the two input files
run: $(PROG)
	./$(PROG) faculty.txt classes.txt

# Use Python to turn the output into Word and Excel files
docs: run
	-python3 make_docs.py

clean:
	rm -f *.o $(LIB) $(PROG) schedule_report.txt schedule_data.txt schedule.docx schedule.xlsx