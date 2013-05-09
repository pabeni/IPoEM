SRCS=core.c
OBJS=$(SRCS:.c=.o)
CPPFLAGS += -Wp,-MD,-MG
CFLAGS += -g -Wall

all: mailtun

mailtun: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $? $(LIBS)

clean:
	rm -f *.o mailtun
