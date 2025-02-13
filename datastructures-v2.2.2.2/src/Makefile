# List of directories containing sub-projects
SUB_DIRS := array_1d array_2d dlist int_array_1d int_list int_list_array int_stack list int_queue queue stack table

# Rule to call 'make all' in each sub-directory
all:
	for dir in $(SUB_DIRS); do \
		$(MAKE) -C $$dir all; \
	done

clean:
	for dir in $(SUB_DIRS); do \
		$(MAKE) -C $$dir clean; \
	done
