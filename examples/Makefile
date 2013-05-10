CPPFLAGS += -Wp,-MD,-MG
CFLAGS += -g -Wall
LIBS+= -lcurl

all: fetchmail

fetchmail: fetchmail.c
	$(CC) $(LDFLAGS) -o $@ $? $(LIBS)

clean:
	rm -f *.o fetchmail
