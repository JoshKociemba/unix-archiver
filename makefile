CC = icc
CFLAGS = -Wall -std=c99 -openmp -O3 -g -I.
#overkill on the flags, but that doesn't hurt anything
LDFLAGS = -lrt -lpthread -lm
#replace this if you want to change the output name
TARGET = myar

#any headers go here
INCLUDES = myar.h

#any .c or .cpp files go here
SOURCE = myar.c

#default is to compile
default:	compile

#depends on all of you source and header files
compile: ${SOURCE} ${INCLUDES}
#this assumes you actually are linking all of the source files together
	${CC} ${CFLAGS} ${SOURCE} -o ${TARGET} ${LDFLAGS}

debug: ${SOURCE} ${INCLUDES}
	${CC} ${CFLAGS} ${SOURCE} -o ${TARGET} ${LDFLAGS} -DDEBUG
