include ../config.mk

LIB_STATIC = libwgtk.a

# ls *.c | sed "s%\.c%.o%g" | tr "\n" " " >> Makefile
OBJS = w_gtk.o w_gtk_menu.o

#CPPFLAGS += -I..
CFLAGS += -I.. $(VTE_CFLAGS)
LIBS   += $(VTE_LIBS)

all: $(LIB_STATIC)  

$(LIB_STATIC): $(OBJS)
	$(AR) rcs $(LIB_STATIC) $(OBJS)

test-static: $(LIB_STATIC)
	$(CC) $(CFLAGS) test.c -o $@ $(LDFLAGS) $(LIBS) $(LIB_STATIC)

strip:

distclean: clean

clean:
	rm -f *.o *~ *.a *.so test-static test-shared

install: $(LIB_STATIC)
	mkdir -p $(DESTDIR)$(libdir)
	install -c $(LIB_STATIC) $(DESTDIR)$(libdir)

install-strip: install

uninstall:
	rm -f $(DESTDIR)$(libdir)/$(LIB_STATIC)*


