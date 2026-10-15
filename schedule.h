#ifndef SCHEDULE_H
#define SCHEDULE_H

#define MAX_NAME 64
#define MAX_CODE 16
#define MAX_ROOM 16
#define MAX_OFFICE_BLOCKS 5
#define MAX_COURSES 50
#define MAX_COURSES_PER_PROF 10

/*Time is on 12 hour system so it needs to be stored in minutes, starting at midnight*/
#define DAY_START 540
#define DAY_END 1020

/*Office hour days are defined in same position as the field- MTWTF, where M=0, T=1, etc.*/
typedef struct{
  int day;
  int start;
  int end;
}OfficeHour; 

typedef struct {
    char name[MAX_NAME];
    char department[MAX_NAME];
    char course_codes[MAX_COURSES_PER_PROF][MAX_CODE];
    int  num_courses;
    int  office_hours_needed;
  char office[MAX_ROOM];
  
  OfficeHour office_hours[MAX_OFFICE_BLOCKS];
  int num_office_hours;
} Professor;

typedef struct {
    char code[MAX_CODE];
    char title[MAX_NAME];
    char professor[MAX_NAME];
    char days[6];
    char start_time[8];
    char end_time[8];
    char room[MAX_ROOM];
  int start_min;
  int end_min;
} Course;

#define MAX_COURSES 50
#define MAX_PROFESSORS 20

typedef struct {
    Course list[MAX_COURSES];
    int count;
} CourseList;

typedef struct {
    Professor list[MAX_PROFESSORS];
    int count;
} ProfessorList;

#endif