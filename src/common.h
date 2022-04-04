
#ifndef LXTERMINAL_COMMON_H
#define LXTERMINAL_COMMON_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ENABLE_NLS
#include <locale.h>
#include <libintl.h>
#define _(x) gettext(x)
#define N_(x) (x)
#else
#define _(x) (x)
#define N_(x) (x)
#define gettext(x) (x)
#endif

//#include "gtkcompat.h"
#include "w_gtk.h"
#include <vte/vte.h>

#include "lxterminal.h"
#include "setting.h"

//#include "preferences.h"
extern void terminal_preferences_dialog (GtkMenuItem * action, LXTerminal * terminal);
extern gint terminal_tab_get_position_id (gchar * position);

//#include "unixsocket.h"
extern gboolean lxterminal_socket_initialize(LXTermWindow* lxtermwin, gint argc, gchar** argv);

#endif
