PROJECT = mysync
HEADERS = $(PROJECT).h
OBJ     = mysync.o parse_file_info.o parse_options.o globals.o compare_files.o copy_files.o usage_and_verbosity.o patterns.o

C11     = gcc -std=c11
CFLAGS  = -Wall -Werror

$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)
