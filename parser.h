#ifndef PARSER_H
#define PARSER_H

#include "scheduler.h"

int parse_professors(const char *filename, ProfessorList *profs);
int parse_courses(const char *filename, CourseList *courses);

int time_to_minutes(const char *text);

#endif