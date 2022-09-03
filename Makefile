#MACROS
SHELL=/bin/sh
CC=gcc
INCLUDES=
OBJS= MAIN.C
OUTDIR=BIN/fusb
FILE=-h
BINPATH=/usr/bin

out.o:${OBJS}
	 ${CC} -w ${OBJS} -o ${OUTDIR} && ./${OUTDIR} ${FILE}


install:
	  ${CC} ${OBJS} -o ${OUTDIR} && cp ${OUTDIR} ${BINPATH}
