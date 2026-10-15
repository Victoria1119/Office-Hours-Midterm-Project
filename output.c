/*
 * output.c purpose- prints the schedule tables and saves data file
 */
#include <stdio.h>
#include <string.h>
#include "output.h"

static const char *DAY_NAMES[5] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
static const char DAY_LETTERS[6] = "MTWRF";

void minutes_to_text(int minutes, char *buffer)
{
    int hour = minutes / 60;
    int minute = minutes % 60;
    const char *ampm = (hour >= 12) ? "PM" : "AM";

    if (hour > 12)
        hour -= 12;
    if (hour == 0)
        hour = 12;
    sprintf(buffer, "%d:%02d %s", hour, minute, ampm);
}

void days_to_text(const char *days, char *buffer)
{
    int i, first = 1;
    buffer[0] = '\0';
    for (i = 0; i < 5 && days[i] != '\0'; i++) {
        if (days[i] == '-')
            continue;
        if (!first)
            strcat(buffer, " & ");
        strcat(buffer, DAY_NAMES[i]);
        first = 0;
    }
}

/* Office hours with the same times on different days are shown on one row*/
static int group_office_hours(const Professor *p, char row_days[][6],
                              int row_start[], int row_end[])
{
    int rows = 0, i, j, found;

    for (i = 0; i < p->num_office_hours; i++) {
        const OfficeHour *oh = &p->office_hours[i];
        found = 0;
        for (j = 0; j < rows; j++) {
            if (row_start[j] == oh->start && row_end[j] == oh->end) {
                row_days[j][oh->day] = DAY_LETTERS[oh->day];
                found = 1;
            }
        }
        if (!found) {
            strcpy(row_days[rows], "-----");
            row_days[rows][oh->day] = DAY_LETTERS[oh->day];
            row_start[rows] = oh->start;
            row_end[rows] = oh->end;
            rows++;
        }
    }
    return rows;
}

void print_report(FILE *out, const ProfessorList *profs, const CourseList *courses)
{
    char days[80], start[12], end[12], times[120], course[100];
    char row_days[MAX_OFFICE_BLOCKS][6];
    int row_start[MAX_OFFICE_BLOCKS], row_end[MAX_OFFICE_BLOCKS];
    int i, j, rows;

    for (i = 0; i < profs->count; i++) {
        const Professor *p = &profs->list[i];

        fprintf(out, "============================================================================\n");
        fprintf(out, "%s - %s\n", p->name, p->department);
        fprintf(out, "============================================================================\n");

        fprintf(out, "CLASS SCHEDULE\n");
        fprintf(out, "%-36s %-42s %s\n", "Course", "Times", "Location");
        for (j = 0; j < courses->count; j++) {
            const Course *c = &courses->list[j];
            if (strcmp(c->professor, p->name) != 0)
                continue;
            days_to_text(c->days, days);
            minutes_to_text(c->start_min, start);
            minutes_to_text(c->end_min, end);
            sprintf(course, "%s (%s)", c->code, c->title);
            sprintf(times, "%s %s to %s", days, start, end);
            fprintf(out, "%-36s %-42s %s\n", course, times, c->room);
        }

        fprintf(out, "\nOFFICE HOURS (%d hours per week)\n", p->office_hours_needed);
        fprintf(out, "%-36s %-42s %s\n", "Days", "Times", "Location");
        rows = group_office_hours(p, row_days, row_start, row_end);
        for (j = 0; j < rows; j++) {
            days_to_text(row_days[j], days);
            minutes_to_text(row_start[j], start);
            minutes_to_text(row_end[j], end);
            sprintf(times, "%s to %s", start, end);
            fprintf(out, "%-36s %-42s %s\n", days, times, p->office);
        }

        fprintf(out, "\nNOTE: Instructors are not to be interrupted during class times.\n\n");
    }
}

int write_data_file(const char *filename, const ProfessorList *profs, const CourseList *courses)
{
    FILE *fp = fopen(filename, "w");
    char days[80], start[12], end[12];
    char row_days[MAX_OFFICE_BLOCKS][6];
    int row_start[MAX_OFFICE_BLOCKS], row_end[MAX_OFFICE_BLOCKS];
    int i, j, rows;

    if (fp == NULL) {
        printf("Could not create %s\n", filename);
        return -1;
    }

    /* format: professor|department|CLASS or OFFICE|course or days|times|location */
    for (i = 0; i < profs->count; i++) {
        const Professor *p = &profs->list[i];

        for (j = 0; j < courses->count; j++) {
            const Course *c = &courses->list[j];
            if (strcmp(c->professor, p->name) != 0)
                continue;
            days_to_text(c->days, days);
            minutes_to_text(c->start_min, start);
            minutes_to_text(c->end_min, end);
            fprintf(fp, "%s|%s|CLASS|%s (%s)|%s %s to %s|%s\n",
                    p->name, p->department, c->code, c->title, days, start, end, c->room);
        }

        rows = group_office_hours(p, row_days, row_start, row_end);
        for (j = 0; j < rows; j++) {
            days_to_text(row_days[j], days);
            minutes_to_text(row_start[j], start);
            minutes_to_text(row_end[j], end);
            fprintf(fp, "%s|%s|OFFICE|%s|%s to %s|%s\n",
                    p->name, p->department, days, start, end, p->office);
        }
    }

    fclose(fp);
    return 0;
}
