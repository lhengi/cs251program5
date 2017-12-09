#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hmap.h"


/**
 * file:  dval_demo.c
 *
 * desc:  demo program using hmap with double
 *        values.
 *
 *        Program just sets a few entries in the hmap
 *        (as pretend gpas).
 *
 *
 * NOTES:
 *
 *        hmap and target must be compiled accordingly to have double
 *        values.  gcc invoked with -DDVAL flag
 *        See makefile. 
 *
 */





int main(int argc, char *argv[]) {
  HMAP * name2gpa = hmap_create(0,1.0);
  double gpa;


  hmap_set(name2gpa, "Bob", 2.5);
  hmap_set(name2gpa, "Mary", 3.5);
  hmap_set(name2gpa, "Sunil", 3.0);
  hmap_set(name2gpa, "Linus", 2.0);
  hmap_set(name2gpa, "Sara", 4.0);


  gpa = hmap_get(name2gpa, "Mary");

  printf("GPA retrieved for Mary: %f\n", gpa);

  hmap_set(name2gpa, "Mary", gpa+.2);

  gpa = hmap_get(name2gpa, "Mary");

  printf("GPA retrieved for Mary after change: %f\n", gpa);

  hmap_free(name2gpa,0);


}
