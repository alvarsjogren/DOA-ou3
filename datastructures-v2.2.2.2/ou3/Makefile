

CC = gcc
CFLAGS = -std=c99 -Wall -I../include -g

all:	tabletest-table2 tabletest-mtftable tabletest-arraytable

tabletest-table2: tabletest-1.10.c ../src/table/table2.c ../src/dlist/dlist.c
	gcc -o $@ $(CFLAGS) $^

tabletest-mtftable: tabletest-1.10.c mtftable.c ../src/dlist/dlist.c
	gcc -o $@ $(CFLAGS) $^

tabletest-arraytable: tabletest-1.10.c arraytable.c ../src/array_1d/array_1d.c
	gcc -o $@ $(CFLAGS) $^