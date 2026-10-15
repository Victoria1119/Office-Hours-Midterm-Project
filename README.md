For this project I wrote a C program that reads two files, faculty.txt and classes.txt, and builds a schedule for each professor. It lists their classes and gives them office hours that don't overlap any of their classes. I also used Python to turn the results into Word and Excel files, and to turn the schedule into Prolog facts and rules so Prolog can answer questions.

Tools Used --------------
I built and gtested this program on Windows using WSL (ubuntu Linux). I used the following commands:
$sudo apt update
$sudo apt install -y build-essential swi-prolog python3-pip python3-docx python3-openpyxl
With this install, the build essentials allowed me to code the C language part of the project, SWI-Prolog allowed me to use Prolog, and Python commands made the output viewable in Word and Excel files.

I also used a pip command to install pyswip, to let Python ask Prolog the questions.

I made this program compatible with CoCalc online Linux environment as well. Since the free version of CoCalc does not have internet connection, pip install and Prolog do not work in that environment. Because of this, I wrote some lines in the Makefile to start with a hyphen, so that the program skips Python- and Prolog-related lines without breaking. If this program is run in Cocalc, the schedule is still built, but outpuut cannot be exported to any external documents.

Running the Code---------
In Ubuntu: 
Using the $make command runs the entire program. This command will build the static library from .c files turned into .o files (parser, scheduler, output files). It lets the main file build the schedule. It then runs all input text files and Python files to create an Excel and Word output. Finally, it allows Python to connect with Prolog in order to answer predetermined questions about the schedule. To delete everything that was created in the program, use the command $make clean in the command prompt.

Files-------------------

libschedule.a: Static library
schedule.h	Structs: Professor, Course, OfficeHour, ProfessorList, CourseList
main.c:	Gets two file names from the command line and calls the other modules
parser.c / parser.h:	Reads faculty.txt and classes.txt; converts times to minutes
scheduler.c / scheduler.h:	Assigns office hours that don't overlap classes
output.c / output.h:	Prints the report and writes schedule_data.txt (sample output textfile)
make_schedule.py:	Turns schedule_data.txt into schedule.docx and schedule.xlsx
Makefile	Builds and runs all files
faculty.txt	Input file 1 for professors
classes.txt	Input file 2 for classes
ask_prolog.py: asks Prolog questions from Python with pyswip
queries.pl: The questions Prolog answers

Input formats---------------
Input files use a pipe as a delimiter. Both files are stored locally in the project folder, so the program works without the internet.

Output
schedule_report.txt: the class schedule and office hours tables
schedule_data.txt: the same results in | format, used by Python
schedule.docx and schedule.xlsx: Word and Excel versions
