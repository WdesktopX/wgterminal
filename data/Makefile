
include ../config.mk

all:

strip:

install:
	mkdir -p $(DESTDIR)$(datadir)/applications
	$(INSTALL_DATA) lxterminal.desktop $(DESTDIR)$(datadir)/applications
	mkdir -p $(DESTDIR)$(datadir)/icons/hicolor/128x128/apps
	$(INSTALL_DATA) lxterminal.png $(DESTDIR)$(datadir)/icons/hicolor/128x128/apps
	mkdir -p $(DESTDIR)$(mandir)/man1
	$(INSTALL_DATA) lxterminal.1 $(DESTDIR)$(mandir)/man1
	mkdir -p $(DESTDIR)$(datadir)/lxterminal
	$(INSTALL_DATA) lxterminal.conf $(DESTDIR)$(datadir)/lxterminal
	$(INSTALL_DATA) lxterminal-preferences.ui $(DESTDIR)$(datadir)/lxterminal

install-strip: install

uninstall:
	rm -f $(DESTDIR)$(datadir)/applications/lxterminal.desktop
	rm -f $(DESTDIR)$(datadir)/icons/hicolor/128x128/apps/lxterminal.png
	rm -f $(DESTDIR)$(mandir)/man1/lxterminal.1
	rm -rf $(DESTDIR)$(datadir)/lxterminal

clean:

distclean:

