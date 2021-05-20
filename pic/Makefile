SRC := array.cc pic.cc init.cc step.cc output.cc
OBJ := $(SRC:%.cc=%.o)

test:  pic
	./pic

$(OBJ): %.o: %.cc pic.h array.h
	g++ -Wall -O2 -c $<

pic:  $(OBJ)
	g++ -o pic $^

clean:
	rm -f pic $(OBJ) data/partData.* data/fieldData.*



