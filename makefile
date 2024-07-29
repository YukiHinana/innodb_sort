innodb_sort: sort_page.o
	gcc sort_page.o -o innodb_sort
clean:
	rm -rf *.o
	rm -f innodb_sort