#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include "schedule.h"

void minutes_to_text(int minutes, char *buffer);


void days_to_text(const char *days, char *buffer);

/* Prints the CLASS SCHEDULE and OFFICE HOURS tables for each professor.
   Use stdout for the screen, or an open file to save the report. */
void print_report(FILE *out, const ProfessorList *profs, const CourseList *courses);

/* Saves the results in a '|' separated file for the Python script. */
int write_data_file(const char *filename, const ProfessorList *profs, const CourseList *courses);

#endif