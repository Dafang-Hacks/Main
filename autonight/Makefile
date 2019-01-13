
CFLAGS=-std=c99 -muclibc -O2 -lrt

autonight: autonight.c
	$(CC) $(CFLAGS) -o $@ $<


.PHONY: clean
clean:
	rm -f autonight
