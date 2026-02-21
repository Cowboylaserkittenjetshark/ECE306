#ifndef SHAPES_H
#define SHAPES_H

#include <stdio.h>

#define WAIT_TIME      (500)
#define PERIOD         (10)
#define CIRCLE_PERIODS (50)

typedef enum shape {
    NONE, CIRCLE, TRIANGLE, FEIGHT
} Shape;

typedef enum state {
    WAIT, START, RUN, END
} State;

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
