

Hash table implementation of a map ADT.

Data structure implements KEY-to-VALUE maps:


    keys:     always strings.

    values:   can be one of the following:

                integer
                double
                char *
                void *  (most generic config).

IMPORTANT:  selection of value-type is determined via 
            CONDITIONAL COMPILATION
            
            The gcc flags used for each configuration are
            as follows:

            VALUE-TYPE           gcc flag
            -------------------------------
              integer           -DIVAL
              double            -DDVAL
              char *            -DSVAL
              void *            -DPVAL

            
            Flag must be used both when compiling hmap.c and
            the application program.

            See makefile

Main files:

  hmap.h  :  specifies the interface to hmap ADT

  hmap.c  :  the implementation of the ADT

  test.c  :  toy driver program which reads a dictionary and 
              produces a report.
             Experiments with different hash functions.
             Uses hmap as a set (value not relevant).

  freq.c  :  demo program using hmap with integer values.
              reads strings from standard input and tracks
              the number of occurrences of each string as an
              integer (the value associated with each distinct
              string).

  dval_demo.c:  demo program using doubles as values (made up
                gpa database).

  pval_demo.c:  demo program using void pointers as values. 
                Associates with each key a linked list.
                Deomonstrates how memory management of such 
                generic data can be handled.

  makefile


Details on test.c:


Simple test program test.c reads a dictionary from standard input and populates
several hmaps (where each hmap uses a different hash function).  Then prints a crude
ASCII visualization of the table list lengths.  The following will read the dictionary
dictionary-small.txt and print a summary of table statistics to the file 'profile'

    ./test < dictionary-small.txt > profile

Options to test program:

    -d : output displays the 'collision profile' of each table -- i.e., a 
          sideways histogram showing the number of elements in each 
          array entry.

    -c <coeff> : lets the user set the scaling coefficient for the default
                hash function to positive integer of their choice.
                Default value is 27 (see hmap.h)

Dictionary files:

    dictionary-small.txt
    dictionary-big.txt
