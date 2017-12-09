
hmap_int.o: hmap.c hmap.h
	gcc -DIVAL -c -g hmap.c -o hmap_int.o

hmap_dbl.o: hmap.c hmap.h
	gcc -DDVAL -c -g hmap.c -o hmap_dbl.o

hmap_str.o: hmap.c hmap.h
	gcc -DSVAL -c -g hmap.c -o hmap_str.o

hmap_ptr.o: hmap.c hmap.h
	gcc -DPVAL -c -g hmap.c -o hmap_ptr.o

hmap.o: hmap.c hmap.h
	gcc -c -g hmap.c

fakemake: fakemake.c hmap.h hmap_ptr.o
	gcc -DPVAL -g fakemake.c hmap_ptr.o -o fakemake

clean:
	rm -f *.o fakemake

