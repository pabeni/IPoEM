CPPFLAGS += -Wp,-MD,-MG
CFLAGS += -g -Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/
LDFLAGS += -L/usr/lib/x86_64-linux-gnu/
LIBS+= -lcurl -lyaml -lglib-2.0

all: fetchmail config

fetchmail: fetchmail.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $? $(LIBS)
	
config: config.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $? $(LIBS)

clean:
	rm -f *.o fetchmail
