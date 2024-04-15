CC=g++

DEPS=MPU6050G2.h
CFLAGS=-fPIC -Wall

LIBS=-li2c
LFLAGS=-shared

OBJ=MPU6050G2.o
OLIB=libMPU6050G2.so



%.o: %.cpp $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

all: $(OBJ)
	$(CC) -o $(OLIB) $< $(LIBS) $(LFLAGS)

install: all $(OLIB) $(DEPS)
	install -m 755 -p $(OLIB) /usr/lib/
	install -m 644 -p $(DEPS) /usr/include/

uninstall:
	rm -f /usr/include/MPU6050G2.h
	rm -f /usr/lib/libMPU6050G2.so

clean:
	rm -f ExampleG2
	rm -f ExampleG2.o
	rm -f MPU6050G2.o
	rm -f libMPU6050G2.so

example: ExampleG2.o
	$(CC) $< -o ExampleG2 -lMPU6050G2 -pthread
