include ../config.mk

APP  = lxterminal
WGTK = ../libwgtk

WOBJ = $(WGTK)/w_gtk.o $(WGTK)/w_gtk_menu.o
OBJS = $(WOBJ) lxterminal.o preferences.o setting.o unixsocket.o 

LDFLAGS += 
#CPPFLAGS += -I..
CFLAGS += -I.. -I$(WGTK) $(VTE_CFLAGS)
LIBS   += $(VTE_LIBS) -lX11

all: $(APP)

$(APP): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(APP) $(LDFLAGS) $(LIBS)

strip: all
	$(STRIP) $(APP)

distclean: clean

clean:
	rm -f *.o *~ $(APP) $(APP).exe  $(WGTK)/*.o

install: all
	mkdir -p $(DESTDIR)$(bindir)
	install -c $(APP) $(DESTDIR)$(bindir)

install-strip: strip install

uninstall:
	rm -f $(DESTDIR)$(bindir)/$(APP)
