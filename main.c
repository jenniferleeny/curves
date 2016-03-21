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
    
  points = new_matrix(4,4);
  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);
  
  //add_curve(points, 150, 150, 150, 50, 350, 150, 350, 300, 0.1, 0);
  add_curve(edges, 200, 250, 150, 50, 300, 250, 300, 250, 0.01, 1);
  draw_lines(edges, s, c);

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
