#include "Include/shapes.h"
#include "Include/global.h"
#include "Include/motors.h"
#include <string.h>
#include <stdbool.h>

#define CIRC_RIGHT_DUTY (0)
#define CIRC_LEFT_DUTY  (0)

// Shape to run
Shape shape = NONE;
State state = WAIT;

// Time elapsed waiting to start
volatile unsigned int wait_time_elapsed = 0;

// Number of periods of length PERIOD elapsed
volatile unsigned int period_count = 0;

// Duty cycles of motors
volatile unsigned int right_duty = 0;
volatile unsigned int left_duty = 0;

// Number of cycles motor has been on
volatile unsigned int right_count = 0;
volatile unsigned int left_count = 0;


void shape_state_machine(void){
  switch(state) {
    case WAIT:
      wait();
      break;
    case START:
      start();
      break;
    case RUN:
      switch(shape) {
        case CIRCLE:
          run_circle();
          break;
        case TRIANGLE:
          run_triangle();
          break;
        case FEIGHT:
          run_figure_eight();
          break;
        case NONE:
          set_state(END);
          break;
        default: break;
      }
      break;
    case END:
      end();
      break;
    default: break;
  }  
}

void wait(void) {
  if(time_change) {
    time_change = false;
    if(shape != NONE) {
      if(wait_time_elapsed >= WAIT_TIME) {
        // Reset time, go to next state
        wait_time_elapsed = 0;
        set_state(START);
      } else wait_time_elapsed += 1;
    } else wait_time_elapsed = 0;
  }
}

void start(void) {
  if(time_change) {
    time_change = false;
    period_count = 0;
    cycle_time = 0;
    right_duty = 0;
    left_duty = 0;
    right_count = 0;
    left_count = 0;
    motors_forward();
    set_state(RUN);
  }
}

void run_circle(void) {
  if(time_change) {
    time_change = false;
    if(period_count <= CIRCLE_PERIODS) {
      right_count += 1;
      if(right_count >= CIRC_RIGHT_DUTY) right_motor_off();

      left_count += 1;
      if(left_count >= CIRC_LEFT_DUTY) left_motor_off();

      // Next period
      if(cycle_time >= PERIOD) {
        cycle_time = 0;
        right_count = 0;
        left_count = 0;
        period_count += 1;
        motors_forward();
      }
    } else set_state(END);
  }
}

void run_triangle(void) {
  
}

void run_figure_eight(void) {
  
}

void end(void) {
  if(time_change) {
    time_change = false;
    motors_off();
    set_state(WAIT);
  }
}

void next_shape(void) {
  switch(shape) {
    case NONE:
      set_shape(CIRCLE);
      break;
    case CIRCLE:
      set_shape(TRIANGLE);
      break;
    case TRIANGLE:
      set_shape(FEIGHT);
      break;
    case FEIGHT:
      set_shape(NONE);
      break;
    default:
      set_shape(NONE);
      break;
  }
}

void prev_shape(void) {
  switch(shape) {
    case NONE:
      set_shape(FEIGHT);
      break;
    case CIRCLE:
      set_shape(NONE);
      break;
    case TRIANGLE:
      set_shape(CIRCLE);
      break;
    case FEIGHT:
      set_shape(TRIANGLE);
      break;
    default:
      set_shape(NONE);
      break;
  }
}

void set_shape(Shape s) {
  shape = s;
  switch(shape) {
    case NONE:
      strcpy(display_line[1], "   NONE   ");
      break;
    case CIRCLE:
      strcpy(display_line[1], "  CIRCLE  ");
      break;
    case TRIANGLE:
      strcpy(display_line[1], " TRIANGLE ");
      break;
    case FEIGHT:
      strcpy(display_line[1], " FIGURE 8 ");
      break;
    default:
      strcpy(display_line[1], "   NONE   ");
      break;
  }

  display_changed = true;
}
  
void set_state(State s) {
  state = s;
  switch(state) {
    case WAIT:
      strcpy(display_line[0], " WAIT     ");
      break;
    case START:
      strcpy(display_line[0], "  START   ");
      break;
    case RUN:
      strcpy(display_line[0], "  RUN     ");
      break;
    case END:
      strcpy(display_line[0], "  END     ");
      break;
    default:
      strcpy(display_line[0], "  INVALID ");
      break;
  }

  display_changed = true;
}
