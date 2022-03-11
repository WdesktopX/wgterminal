include ../config.mk

APP  = lxterminal
# ls *.c | sed "s%\.c%.o%g" | tr "\n" " " >> Makefile
OBJS = lxterminal.o preferences.o setting.o tab.o unixsocket.o 

LDFLAGS +=  $(PACKAGE_LIBS) $(INTLLIBS)
#CPPFLAGS += -I..
CFLAGS += -I.. 
LIBS   += 

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
