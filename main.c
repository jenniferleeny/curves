#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main( int argc, char **argv ) {

  screen s;
  clear_screen(s);
  color c;
  c.red = 225;
  c.blue = 0;
  c.green = 0;

  struct matrix *points;
  struct matrix *edges;
  struct matrix *transform;
  
  //make_hermite();
  //printf("\n");
  //print_matrix(make_hermite());
  
  //points = generate_curve_coefs(100, 150, 200, 250, 1);
  //print_matrix(points);

  points = new_matrix(4,4);
  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);
  
  
  //add_circle(points, 250, 250, 100, 0.1);
  add_curve(points, 150, 150, 150, 50, 350, 150, 350, 300, 0.1, 0);
  draw_lines(points, s, c);

  display(s);
  /*
  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );
  
  free_matrix( transform );
  free_matrix( edges );
  */
}
