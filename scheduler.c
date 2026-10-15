
#include <stdio.h>
#include <string.h>
#include "scheduler.h"

/* 1 if this course meets on this day (day 0 = M, 1 = T, 2 = W, 3 = R, 4 = F) */
static int meets_on(const Course *c, int day)
{
    return c->days[day] != '-' && c->days[day] != '\0';
}

/* Two time ranges overlap if each one starts before the other one ends. */
static int overlaps(int start1, int end1, int start2, int end2)
{
    return start1 < end2 && start2 < end1;
}

int is_free(const Professor *p, int day, int start, int end, const CourseList *courses)
{
    int i;

    if (start < DAY_START || end > DAY_END)
        return 0;

    /* no overlap with any of this professor's classes on this day */
    for (i = 0; i < courses->count; i++) {
        const Course *c = &courses->list[i];
        if (strcmp(c->professor, p->name) == 0 && meets_on(c, day) &&
            overlaps(start, end, c->start_min, c->end_min))
            return 0;
    }

    /* no overlap with office hours already given */
    for (i = 0; i < p->num_office_hours; i++) {
        if (p->office_hours[i].day == day &&
            overlaps(start, end, p->office_hours[i].start, p->office_hours[i].end))
            return 0;
    }
    return 1;
}

static void add_block(Professor *p, int day, int start, int end)
{
    if (p->num_office_hours < MAX_OFFICE_BLOCKS) {
        p->office_hours[p->num_office_hours].day = day;
        p->office_hours[p->num_office_hours].start = start;
        p->office_hours[p->num_office_hours].end = end;
        p->num_office_hours++;
    }
}

static void schedule_one(Professor *p, const CourseList *courses)
{
    int teaches[5] = {0, 0, 0, 0, 0};
    int num_days = 0, day, i, start, length, placed;

    /* which days does this professor teach? */
    for (i = 0; i < courses->count; i++) {
        if (strcmp(courses->list[i].professor, p->name) != 0)
            continue;
        for (day = 0; day < 5; day++)
            if (meets_on(&courses->list[i], day))
                teaches[day] = 1;
    }
    for (day = 0; day < 5; day++)
        num_days += teaches[day];

    /* a professor with no classes can hold office hours any weekday */
    if (num_days == 0) {
        for (day = 0; day < 5; day++)
            teaches[day] = 1;
        num_days = 5;
    }

    length = p->office_hours_needed * 60 / num_days;   /* minutes per day */
    if (length <= 0)
        return;

    for (day = 0; day < 5; day++) {
        if (!teaches[day])
            continue;
        placed = 0;

        /* try 1: right after one of their classes */
        for (i = 0; i < courses->count && !placed; i++) {
            const Course *c = &courses->list[i];
            if (strcmp(c->professor, p->name) == 0 && meets_on(c, day) &&
                is_free(p, day, c->end_min, c->end_min + length, courses)) {
                add_block(p, day, c->end_min, c->end_min + length);
                placed = 1;
            }
        }

        /* try 2: first free time from 9:00 AM, every 15 minutes */
        for (start = DAY_START; !placed && start + length <= DAY_END; start += 15) {
            if (is_free(p, day, start, start + length, courses)) {
                add_block(p, day, start, start + length);
                placed = 1;
            }
        }

        if (!placed)
            printf("Warning: no free time for %s on day %d\n", p->name, day);
    }
}

void assign_office_hours(ProfessorList *profs, const CourseList *courses)
{
    int i;
    for (i = 0; i < profs->count; i++)
        schedule_one(&profs->list[i], courses);
}
