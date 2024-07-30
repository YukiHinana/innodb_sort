CFLAGS = -g -O2

innodb_sort: sort_page.o
	gcc sort_page.o -o innodb_sort

%.o: %.c
	gcc -c $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -rf *.o
	rm -f innodb_sort