#include "include/shapes.h"
#include "include/global.h"
#include "include/motors.h"
#include <stdbool.h>
#include <string.h>

// Shape to run
volatile Shape shape = NONE;
volatile State state = WAIT;

// Triangle things
volatile Triangle_State triangle_state = STRAIGHT;
volatile unsigned int triangle_segment = 0;

// Time elapsed waiting to start
volatile unsigned int wait_time_elapsed = 0;

// Number of periods of length PERIOD elapsed
volatile unsigned int period_count = 0;

void shape_state_machine(void) {
  switch (state) {
  case WAIT:
    wait();
    break;
  case START:
    start();
    break;
  case RUN:
    switch (shape) {
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
    default:
      break;
    }
    break;
  case END:
    end();
    break;
  default:
    break;
  }
}

void wait(void) {
  if (time_change) {
    time_change = false;
    if (shape != NONE) {
      if (wait_time_elapsed >= WAIT_TIME) {
        // Reset time, go to next state
        wait_time_elapsed = 0;
        set_state(START);
      } else
        wait_time_elapsed += 1;
    } else
      wait_time_elapsed = 0;
  }
}

void start(void) {
  if (time_change) {
    time_change = false;
    period_count = 0;
    cycle_time = 0;
    triangle_state = STRAIGHT;
    triangle_segment = 0;
    motors_forward();
    set_state(RUN);
  }
}

void run_circle(void) {
  if (time_change) {
    time_change = false;
    if (period_count <= CIRCLE_PERIODS) {
      if (cycle_time >= CIRCLE_MAJOR_DUTY)
        left_motor_off();
      
      if (cycle_time >= CIRCLE_MINOR_DUTY)
        right_motor_off();

      // Next period
      if (cycle_time >= PERIOD) {
        cycle_time = 0;
        period_count += 1;
        if(CIRCLE_MAJOR_DUTY > 0) left_motor_forward();
        if(CIRCLE_MINOR_DUTY > 0) right_motor_forward();
      }
    } else
      set_state(END);
  }
}

void run_triangle(void) {
  if(time_change) {
    time_change = false;
    switch (triangle_state) {
      case STRAIGHT:
        if(period_count <= TRIANGLE_STRAIGHT_PERIODS) {
          if(cycle_time >= TRIANGLE_STRAIGHT_DUTY) motors_off();
          if(cycle_time >= PERIOD) {
            cycle_time = 0;
            period_count += 1;
            if(TRIANGLE_STRAIGHT_DUTY > 0) motors_forward();
          }
        } else if (triangle_segment < 5) {
          triangle_state = TURN;
          triangle_segment += 1;
          period_count = 0;
        } else set_state(ERR);
        break;
      case TURN:
        if(period_count <= TRIANGLE_TURN_PERIODS) {
          if (cycle_time >= TRIANGLE_MAJOR_DUTY)
            left_motor_off();
        
          if (cycle_time >= TRIANGLE_MINOR_DUTY)
            right_motor_off();
       
          if(cycle_time >= PERIOD) {
            cycle_time = 0;
            period_count += 1;
            if(TRIANGLE_MAJOR_DUTY > 0) left_motor_forward();
            if(TRIANGLE_MINOR_DUTY > 0) right_motor_forward();
          }
        } else if (triangle_segment < 5) {
          triangle_state = STRAIGHT;
          triangle_segment += 1;
          period_count = 0;
        } else set_state(END);
        break;
      default: break;
    }
  }
}

void run_figure_eight(void) {}

void end(void) {
  if (time_change) {
    time_change = false;
    motors_off();
    set_state(WAIT);
  }
}

void next_shape(void) {
  switch (shape) {
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
  switch (shape) {
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
  switch (shape) {
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
  switch (state) {
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
  case ERROR:
    strcpy(display_line[0], "  ERR     ");
    break;
 default:
    strcpy(display_line[0], "  INVALID ");
    break;
  }

  display_changed = true;
}
