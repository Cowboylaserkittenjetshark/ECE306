#ifndef SHAPES_H
#define SHAPES_H

#include <stdio.h>

#define WAIT_TIME (500)
#define PERIOD (10)
#define CIRCLE_PERIODS (500)
#define CIRCLE_MAJOR_DUTY (10)
#define CIRCLE_MINOR_DUTY (1)
#define TRIANGLE_STRAIGHT_PERIODS (50)
#define TRIANGLE_TURN_PERIODS (50)
#define TRIANGLE_STRAIGHT_DUTY (7)
#define TRIANGLE_MAJOR_DUTY (7)
#define TRIANGLE_MINOR_DUTY (0)

typedef enum shape { NONE, CIRCLE, TRIANGLE, FEIGHT } Shape;

typedef enum state { WAIT, START, RUN, END, ERROR } State;
typedef enum triangle_state { STRAIGHT, TURN } Triangle_State;

void shape_state_machine(void);

void wait(void);
void start(void);
void run_circle(void);
void run_triangle(void);
void run_figure_eight(void);
void end(void);

void next_shape(void);
void prev_shape(void);
void set_shape(Shape s);
void set_state(State s);

#endif
