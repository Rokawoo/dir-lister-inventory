cc := gcc
FLAGS :=
TARGET := 265inventory

view : 
	less 265inventory.c

build : $(TARGET)

$(TARGET) : 265inventory.o
			$(cc) $(FLAGS) -o $@ $^

265inventory.o : 265inventory.c
	$(cc) $(FLAGS) -c -o $@ $<

.PHONY: clean cleanall

clean :
		-\rm -f *.o
		-\rm -f 265inventory
