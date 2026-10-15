#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

/* Copies a token into a field. If the token is missing (NULL), it returns 0
   so the caller can skip the bad line instead of crashing. */
static int copy_token(char *dest, const char *token, int size)
{
    if (token == NULL)
        return 0;
    strncpy(dest, token, size - 1);
    dest[size - 1] = '\0';
    return 1;
}

int time_to_minutes(const char *text)
{
    int hour, minute;
    char ampm[3] = "";

    /* "05:00PM" -> hour = 5, minute = 0, ampm = "PM" */
    if (sscanf(text, "%d:%d%2s", &hour, &minute, ampm) < 2)
        return -1;

    if (strcmp(ampm, "PM") == 0 && hour != 12)
        hour += 12;               /* 1:15PM -> 13:15 */
    if (strcmp(ampm, "AM") == 0 && hour == 12)
        hour = 0;                 /* 12:30AM -> 0:30 */

    return hour * 60 + minute;
}

int parse_professors(const char *filename, ProfessorList *profs) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        return -1;
    }

    char line[256];
    int line_num = 0;
    profs->count = 0;

    while (fgets(line, sizeof(line), fp) != NULL && profs->count < MAX_PROFESSORS) {
        Professor p;
        char *outer_save;
        char *inner_save;
        line_num++;

        line[strcspn(line, "\r\n")] = '\0';     /* remove the newline */
        if (strlen(line) == 0)
            continue;                            
      
        char *token = strtok_r(line, "|", &outer_save);
        if (!copy_token(p.name, token, MAX_NAME)) {
            printf("%s line %d: missing name, skipped\n", filename, line_num);
            continue;
        }

        token = strtok_r(NULL, "|", &outer_save);
        if (!copy_token(p.department, token, MAX_NAME)) {
            printf("%s line %d: missing department, skipped\n", filename, line_num);
            continue;
        }

        token = strtok_r(NULL, "|", &outer_save);
        if (token == NULL) {
            printf("%s line %d: missing courses, skipped\n", filename, line_num);
            continue;
        }
        p.num_courses = 0;
        /* split on comma AND space so "COSC208, COSC214" gives "COSC214", not " COSC214" */
        char *course = strtok_r(token, ", ", &inner_save);
        while (course != NULL && p.num_courses < MAX_COURSES_PER_PROF) {
            strncpy(p.course_codes[p.num_courses], course, MAX_CODE - 1);
            p.course_codes[p.num_courses][MAX_CODE - 1] = '\0';
            p.num_courses++;
            course = strtok_r(NULL, ", ", &inner_save);
        }

        token = strtok_r(NULL, "|", &outer_save);
        if (token == NULL) {
            printf("%s line %d: missing office hours, skipped\n", filename, line_num);
            continue;
        }
        p.office_hours_needed = atoi(token);

        /* optional 5th field: office room */
        token = strtok_r(NULL, "|", &outer_save);
        if (!copy_token(p.office, token, MAX_ROOM))
            strcpy(p.office, "TBA");

        p.num_office_hours = 0;                  /* the scheduler fills these in */
        profs->list[profs->count] = p;
        profs->count++;
    }

    fclose(fp);
    return 0;
}

int parse_courses(const char *filename, CourseList *courses) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        return -1;
    }

    char line[256];
    int line_num = 0;
    courses->count = 0;

    while (fgets(line, sizeof(line), fp) != NULL && courses->count < MAX_COURSES) {
        Course c;
        char *save;
        line_num++;

        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0)
            continue;

        char *token = strtok_r(line, "|", &save);
        int ok = copy_token(c.code, token, MAX_CODE);

        token = strtok_r(NULL, "|", &save);
        ok = ok && copy_token(c.title, token, MAX_NAME);

        token = strtok_r(NULL, "|", &save);
        ok = ok && copy_token(c.professor, token, MAX_NAME);

        token = strtok_r(NULL, "|", &save);
        ok = ok && copy_token(c.days, token, 6);

        token = strtok_r(NULL, "|", &save);
        ok = ok && copy_token(c.start_time, token, 8);

        token = strtok_r(NULL, "|", &save);
        ok = ok && copy_token(c.end_time, token, 8);

        token = strtok_r(NULL, "|", &save);
        ok = ok && copy_token(c.room, token, MAX_ROOM);

        if (!ok) {
            printf("%s line %d: missing fields, skipped\n", filename, line_num);
            continue;
        }

        c.start_min = time_to_minutes(c.start_time);
        c.end_min = time_to_minutes(c.end_time);
        if (c.start_min < 0 || c.end_min <= c.start_min) {
            printf("%s line %d: bad times for %s, skipped\n", filename, line_num, c.code);
            continue;
        }

        courses->list[courses->count] = c;
        courses->count++;
    }

    fclose(fp);
    return 0;
}