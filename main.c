/*
 * main.c purpose - Professor teaching and office-hours schedule
 * 1. Read the two input files            (parser.c)
 * 2. Assign office hours                 (scheduler.c)
 * 3. Print the report and save the files (output.c)
 */
#include <stdio.h>
#include "schedule.h"
#include "parser.h"
#include "scheduler.h"
#include "output.h"

int main(int argc, char *argv[]) {
    ProfessorList profs;
    CourseList courses;

    if (argc != 3) {
        printf("Usage: %s faculty.txt classes.txt\n", argv[0]);
        return 1;
    }

    if (parse_professors(argv[1], &profs) != 0)
        return 1;
    if (parse_courses(argv[2], &courses) != 0)
        return 1;
    printf("Read %d professors and %d courses\n\n", profs.count, courses.count);

    assign_office_hours(&profs, &courses);

    /* print to screen */
    print_report(stdout, &profs, &courses);

    /* save the same report to a file */
    FILE *fp = fopen("schedule_report.txt", "w");
    if (fp == NULL) {
        printf("Could not create schedule_report.txt\n");
        return 1;
    }
    print_report(fp, &profs, &courses);
    fclose(fp);

    /* save the data for the Python script (Word/Excel) */
    if (write_data_file("schedule_data.txt", &profs, &courses) != 0)
        return 1;

    printf("Saved schedule_report.txt and schedule_data.txt\n");
    return 0;
}
