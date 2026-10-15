#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "schedule.h"

/* Returns 1 if the professor is free on that day from start to end. */
int is_free(const Professor *p, int day, int start, int end, const CourseList *courses);

/* Gives every professor office hours that do not overlap their classes. */
void assign_office_hours(ProfessorList *profs, const CourseList *courses);

#endif