CFLAGS = -Wall -Wextra -Wshadow -pedantic
LIB_PATH = ../BBBIOlib/BBBio_lib/
LDFLAGS = -L $(LIB_PATH) -lBBBio

DEPS = libBBBio.a utils.o

all: demo_one_row

demo_one_row: demo_one_row.o $(DEPS)
	gcc $^ $(LDFLAGS) -L ${LIB_PATH} -lBBBio -o $@

demo_cycle: demo_cycle.o $(DEPS)
	gcc $^ $(LDFLAGS) -L ${LIB_PATH} -lBBBio -o $@

libBBBio.a: ${LIB_PATH}BBBiolib.c ${LIB_PATH}BBBiolib.h BBBiolib_PWMSS.o BBBiolib_McSPI.o BBBiolib_ADCTSC.o
	gcc -c ${LIB_PATH}BBBiolib.c -o ${LIB_PATH}BBBiolib.o
	ar -rs ${LIB_PATH}libBBBio.a ${LIB_PATH}BBBiolib.o ${LIB_PATH}BBBiolib_PWMSS.o ${LIB_PATH}BBBiolib_McSPI.o ${LIB_PATH}BBBiolib_ADCTSC.o
	cp ${LIB_PATH}libBBBio.a ./

BBBiolib_PWMSS.o: ${LIB_PATH}BBBiolib_PWMSS.c ${LIB_PATH}BBBiolib_PWMSS.h
	gcc -c ${LIB_PATH}BBBiolib_PWMSS.c -o ${LIB_PATH}BBBiolib_PWMSS.o

BBBiolib_McSPI.o: ${LIB_PATH}BBBiolib_McSPI.c ${LIB_PATH}BBBiolib_PWMSS.h
	gcc -c ${LIB_PATH}BBBiolib_McSPI.c -o ${LIB_PATH}BBBiolib_McSPI.o

BBBiolib_ADCTSC.o: ${LIB_PATH}BBBiolib_ADCTSC.c ${LIB_PATH}BBBiolib_ADCTSC.h
	gcc -c ${LIB_PATH}BBBiolib_ADCTSC.c -o ${LIB_PATH}BBBiolib_ADCTSC.o

%.o: %.c
	gcc $< $(CFLAGS) -I$(dir $<) -c -o $@

.PHONY: clean

clean:
	rm -f *.o *.a demo_cycle demo_one_row
