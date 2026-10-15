This C program reads two input files, faculty.txt and classes.txt. It prints each professor's class schedule and assigns office hours that never overlap their classes.

How to compile and run (CoCalc terminal)
pip install python-docx openpyxl     # only needed once (needs internet)
make

make does everything in order:

compiles each .c file into a .o file
builds the static library libschedule.a from parser.o, scheduler.o and output.o
links main.o with the library to make the program schedule
runs ./schedule faculty.txt classes.txt
runs python3 make_docs.py to make the Word and Excel files

If the computer has no internet, pip install won't work. make still builds and runs the C program; the Python step prints "Missing libraries" and Make continues, because that line in the Makefile starts with -.

To delete everything that was generated: make clean

Files

schedule.h	Structs: Professor, Course, OfficeHour, ProfessorList, CourseList
main.c	Reads the two file names from the command line and calls the other modules
parser.c / parser.h	Reads faculty.txt and classes.txt; converts times to minutes
scheduler.c / scheduler.h	Assigns office hours that don't overlap classes
output.c / output.h	Prints the report and writes schedule_data.txt
make_schedule.py	Turns schedule_data.txt into schedule.docx and schedule.xlsx
Makefile	Builds the library and program, runs the program, runs the Python script
faculty.txt	Input file 1
classes.txt	Input file 2
Input formats
faculty.txt: name | department | courses (comma separated) | office hours per week | office room (optional; "TBA" if missing)

classes.txt: code | title | professor | days | start | end | room

Days are 5 positions, MTWRF (R = Thursday), with "-" for days with no class.

Output
schedule_report.txt: the class schedule and office hours tables
schedule_data.txt: the same results in | format, used by Python
schedule.docx and schedule.xlsx: Word and Excel versions
