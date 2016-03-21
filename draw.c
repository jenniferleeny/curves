#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"


/*======== void add_circle() ==========
  Inputs:   struct matrix * points
            double cx
	        double cy
		    double y
		        double step  
  Returns: 
  03/16/12 19:53:52
  jdyrlandweaver
  ====================*/
void add_circle( struct matrix * points, 
		 double cx, double cy, 
		 double r, double step ) {
  
  double t, x0, x1, y0, y1;
  for (t = 0; t < 1; t+=step) {
    x0 = r*cos(t*2*M_PI) + cx;
    y0 = r*sin(t*2*M_PI) + cy;
    x1 = r*cos((t+step)*2*M_PI) + cx;
    y1 = r*sin((t+step)*2*M_PI) + cy;
    add_edge(points, x0, y0, 0, x1, y1, 0);
    //print_matrix(points);
  }
}

/*======== void add_curve() ==========
Inputs:   struct matrix *points
         double x0
         double y0
         double x1
         double y1
         double x2
         double y2
         double x3
         double y3
         double step
         int type  
Returns: 
Adds the curve bounded by the 4 points passsed as parameters
of type specified in type (see matrix.h for curve type constants)
to the matrix points
03/16/12 15:24:25
jdyrlandweaver
====================*/
void add_curve( struct matrix *points, 
		double x0, double y0, 
		double x1, double y1, 
		double x2, double y2, 
		double x3, double y3, 
		double step, int type ) {
  double a_x, b_x, c_x, d_x;  //coefficients
  double a_y, b_y, c_y, d_y;
  struct matrix * coef_x = new_matrix(4, 4);
  struct matrix * coef_y = new_matrix(4, 4);
  if (type == HERMITE_MODE) {//HERMITE: x0 y0 x2 y2 are the endpoints
    /*    a_x = x0; 
    c_x = x1;
    //3*a + 2*b + c = x3;
    b_x = (x3 - c_x - 3*a_x)/2;
    d_x = x2 - a_x - b_x - c_x;
    
    a_y = y0;
    */
    coef_x = generate_curve_coefs(x0, x2, x1, x3, 0);
    coef_y = generate_curve_coefs(y0, y2, y1, y3, 0);
    a_x = coef_x->m[0][0];
    b_x = coef_x->m[0][1];
    c_x = coef_x->m[0][2];
    d_x = coef_x->m[0][3];
    
    a_y = coef_y->m[0][0];
    b_y = coef_y->m[0][1];
    c_y = coef_y->m[0][2];
    d_y = coef_y->m[0][3];
  }
  else if (type == BEZIER_MODE) {//BEZIER: x0 y0 x3 y3 are the endpoints
    /*    a_x = -x0 + 3*x1 - 3*x2 + x3;
    b_x = 3*x0 + 6*x1 + 3*x2;
    c_x = -3*x0 + 3*x1;
    d_x = x0;
    
    a_y = -y0 + 3*y1 - 3*y2 + y3;
    b_y = 3*y0 + 6*y1 + 3*y2;
    c_y = -3*y0 + 3*y1;
    d_y = y0;
    */
    coef_x = generate_curve_coefs(x0, x1, x2, x3, 0);
    coef_y = generate_curve_coefs(y0, y1, y2, y3, 0);
    a_x = coef_x->m[0][0];
    b_x = coef_x->m[0][1];
    c_x = coef_x->m[0][2];
    d_x = coef_x->m[0][3];

    a_y = coef_y->m[0][0];
    b_y = coef_y->m[0][1];
    c_y = coef_y->m[0][2];
    d_y = coef_y->m[0][3];  
  }
  double t, t1; 
  double point_x0, point_y0, point_x1, point_y1;
  
  for (t = 0; t < 1; t += step) {
    point_x0 = t*(t*(a_x*t + b_x) + c_x) + d_x;
    point_y0 = t*(t*(a_y*t + b_y) + c_y) + d_y;
    t1 = t + step;
    point_x1 = t1*(t1*(a_x*t1 + b_x) + c_x) + d_x;
    point_y1 = t1*(t1*(a_y*t1 + b_y) + c_y) + d_y;
    add_edge(points, x0, y0, 0, x1, y1, 0);
    //add_point(points, x0, y0, 0);
  }
}

/*======== void add_point() ==========
Inputs:   struct matrix * points
         int x
         int y
         int z 
Returns: 
adds point (x, y, z) to points and increment points.lastcol
if points is full, should call grow on points
====================*/
void add_point( struct matrix * points, double x, double y, double z) {
  
  if ( points->lastcol == points->cols )
    grow_matrix( points, points->lastcol + 100 );

  points->m[0][points->lastcol] = x;
  points->m[1][points->lastcol] = y;
  points->m[2][points->lastcol] = z;
  points->m[3][points->lastcol] = 1;

  points->lastcol++;
}

/*======== void add_edge() ==========
Inputs:   struct matrix * points
          int x0, int y0, int z0, int x1, int y1, int z1
Returns: 
add the line connecting (x0, y0, z0) to (x1, y1, z1) to points
should use add_point
====================*/
void add_edge( struct matrix * points, 
	       double x0, double y0, double z0, 
	       double x1, double y1, double z1) {
  add_point( points, x0, y0, z0 );
  add_point( points, x1, y1, z1 );
}

/*======== void draw_lines() ==========
Inputs:   struct matrix * points
         screen s
         color c 
Returns: 
Go through points 2 at a time and call draw_line to add that line
to the screen
====================*/
void draw_lines( struct matrix * points, screen s, color c) {
  int i;
  if ( points->lastcol < 2 ) {
    printf("Need at least 2 points to draw a line!\n");
    return;
  }
  for ( i = 0; i < points->lastcol - 1; i+=2 ) {
    draw_line( points->m[0][i], points->m[1][i], 
	       points->m[0][i+1], points->m[1][i+1], s, c);
  }        
}

void draw_line(int x0, int y0, int x1, int y1, screen s, color c) {
 
  int x, y, d, dx, dy;

  x = x0;
  y = y0;
  
  //swap points so we're always draing left to right
  if ( x0 > x1 ) {
    x = x1;
    y = y1;
    x1 = x0;
    y1 = y0;
  }

  //need to know dx and dy for this version
  dx = (x1 - x) * 2;
  dy = (y1 - y) * 2;

  //positive slope: Octants 1, 2 (5 and 6)
  if ( dy > 0 ) {

    //slope < 1: Octant 1 (5)
    if ( dx > dy ) {
      d = dy - ( dx / 2 );
  
      while ( x <= x1 ) {
	plot(s, c, x, y);

	if ( d < 0 ) {
	  x = x + 1;
	  d = d + dy;
	}
	else {
	  x = x + 1;
	  y = y + 1;
	  d = d + dy - dx;
	}
      }
    }

    //slope > 1: Octant 2 (6)
    else {
      d = ( dy / 2 ) - dx;
      while ( y <= y1 ) {

	plot(s, c, x, y );
	if ( d > 0 ) {
	  y = y + 1;
	  d = d - dx;
	}
	else {
	  y = y + 1;
	  x = x + 1;
	  d = d + dy - dx;
	}
      }
    }
  }

  //negative slope: Octants 7, 8 (3 and 4)
  else { 

    //slope > -1: Octant 8 (4)
    if ( dx > abs(dy) ) {

      d = dy + ( dx / 2 );
  
      while ( x <= x1 ) {

	plot(s, c, x, y);

	if ( d > 0 ) {
	  x = x + 1;
	  d = d + dy;
	}
	else {
	  x = x + 1;
	  y = y - 1;
	  d = d + dy + dx;
	}
      }
    }

    //slope < -1: Octant 7 (3)
    else {

      d =  (dy / 2) + dx;

      while ( y >= y1 ) {
	
	plot(s, c, x, y );
	if ( d < 0 ) {
	  y = y - 1;
	  d = d + dx;
	}
	else {
	  y = y - 1;
	  x = x + 1;
	  d = d + dy + dx;
	}
      }
    }
  }
}
