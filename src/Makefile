include ../config.mk

APP  = lxterminal
# ls *.c | sed "s%\.c%.o%g" | tr "\n" " " >> Makefile
# remove tab.c 
OBJS = lxterminal.o preferences.o setting.o unixsocket.o 

LDFLAGS += 
#CPPFLAGS += -I..
CFLAGS += -I.. -I../libwgtk $(VTE_CFLAGS)
LIBS   += $(VTE_LIBS) -lX11 ../libwgtk/libwgtk.a

all: $(APP)

$(APP): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(APP) $(LDFLAGS) $(LIBS)

strip: all
	$(STRIP) $(APP)

distclean: clean

clean:
	rm -f *.o *~ $(APP) $(APP).exe

install: all
	mkdir -p $(DESTDIR)$(bindir)
	install -c $(APP) $(DESTDIR)$(bindir)

install-strip: strip install

uninstall:
	rm -f $(DESTDIR)$(bindir)/$(APP)
