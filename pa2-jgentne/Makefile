TARGET = pa2
GCC = gcc
CFLAGS = -g -std=c99 -Wall -Wshadow -Wvla -Werror -pedantic
CC = $(GCC) $(CFLAGS)

SRCS = pa2.c tree.c

OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --verbose --log-file

#TESTFLAGS = -DTEST_READHEADER -DTEST_DECODE -DTEST_MERGELIST -DDO_NOT_MODIFY

$(TARGET): $(OBJS)
	$(CC) $(TESTFLAGS) $(OBJS) -o $(TARGET)

all:
	gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -O3 *.c -o pa2

.c.o:
	$(GCC) $(CFLAGS) $(TESTFLAGS) -c $*.c

testmemory: $(TARGET)
	$(VALGRIND)=./valgrind.log ./$(TARGET) -b examples/ops3.b tree3.b

testall: test1 test2 test3 test4 test5 test6 test7

test1: $(TARGET)
	./$(TARGET) -b examples/ops0.b tree0.b
	diff examples/tree0.b tree0.b

test2: $(TARGET)
	./$(TARGET) -b examples/ops1.b tree1.b
	diff examples/tree1.b tree1.b

test3: $(TARGET)
	./$(TARGET) -b examples/ops2.b tree2.b
	diff examples/tree2.b tree2.b

test4: $(TARGET)
	./$(TARGET) -b examples/ops3.b tree3.b
	diff examples/tree3.b tree3.b

test5: $(TARGET)
	./$(TARGET) -e examples/invalidtree0.b

test6: $(TARGET)
	./$(TARGET) -e examples/invalidtree1.b

test7: $(TARGET)
	./$(TARGET) -e examples/invalidtree2.b

clean:
	rm -f $(OBJS) $(TARGET) *.b *.txt
