cc := gcc
FLAGS :=
TARGET := Inventory

view : 
	less Inventory.c

build : $(TARGET)

$(TARGET) : Inventory.o
			$(cc) $(FLAGS) -o $@ $^

Inventory.o : Inventory.c
	$(cc) $(FLAGS) -c -o $@ $<

.PHONY: clean cleanall

clean :
		-\rm -f *.o
		-\rm -f Inventory
