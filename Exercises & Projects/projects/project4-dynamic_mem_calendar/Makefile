CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors -fstack-protector-all -Wextra
PUBTESTS = public01 public02 public03 public04 public05
STUDENT_TESTS = student_tests
OBJECTS = calendar.o my_memory_checker_216.o
HEADERS = event.h calendar.h my_memory_checker_216.h

.PHONY: all clean backup

all: $(PUBTESTS) $(STUDENT_TESTS)

clean:
	rm -f *.o $(PUBTESTS) $(STUDENT_TESTS) a.out

public%: public%.o $(OBJECTS)
	$(CC) -o $@ $^

student_tests: student_tests.o $(OBJECTS)
	$(CC) -o $@ $^

public%.o: public%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

student_tests.o: student_tests.c $(HEADERS)
	$(CC) $(CFLAGS) -c student_tests.c

calendar.o: calendar.c calendar.h
	$(CC) $(CFLAGS) -c calendar.c

my_memory_checker_216.o: my_memory_checker_216.c my_memory_checker_216.h
	$(CC) $(CFLAGS) -c my_memory_checker_216.c