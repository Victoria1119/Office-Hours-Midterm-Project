# Makefile
#   make purpose- compile, build the static library, link, run, make Word/Excel and run the Prolog questions
#   make clean purpose- delete everything that was generated

# Macros
CC     = gcc
CFLAGS = -Wall -g
OBJS   = parser.o scheduler.o output.o
LIB    = libschedule.a
PROG   = schedule

# These targets are actions, not files
.PHONY: all run docs prolog clean

# Default target
all: $(PROG) run docs prolog

# Link main.o with the static library
$(PROG): main.o $(LIB)
	$(CC) $(CFLAGS) main.o -L. -lschedule -o $(PROG)

# Build the static library
$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

# Compile each .c file into a .o file
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
	-python3 make_schedule.py

# Python turns the data into Prolog facts and rules then Prolog answers questions about the schedule
# ask_prolog.py asks the same kind of questions from Python using pyswip.
prolog: run
	python3 make_prolog.py
	swipl -q queries.pl
	-python3 ask_prolog.py

clean:
	rm -f *.o $(LIB) $(PROG) schedule_report.txt schedule_data.txt schedule.docx schedule.xlsx schedule.pl
