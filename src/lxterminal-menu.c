#include "w_gtk_menu.h"

static GtkWidget * vtemenu;
static GtkActionGroup * vtemenu_group;


static GtkWidget * create_popup_menu (GtkActionGroup *action_group, void *user_data)
{
    GtkWidget * menu = gtk_menu_new();

    WGtkMenuItemParams menuitem;
    memset (&menuitem, 0, sizeof(menuitem));

    menuitem.parent_menu = menu;
    menuitem.label       = _("New _Window");
    menuitem.icon_name   = "list-add";
    menuitem.action_name = "NewWindow";
    menuitem.gtk_app     = action_group;
    menuitem.activate_cb = terminal_new_window_activate_event;
    menuitem.cb_data_all = user_data;
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("New _Tab");
    menuitem.icon_name   = "list-add";
    menuitem.action_name = "NewTab";
    menuitem.activate_cb = terminal_new_tab_activate_event;
    w_gtk_menu_item_new (&menuitem);

    /* sep */
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("Open _URL");
    menuitem.action_name = "OpenURL";
    menuitem.activate_cb = terminal_open_url_activate_event;
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("Copy _URL");
    menuitem.action_name = "CopyURL";
    menuitem.activate_cb = terminal_copy_url_activate_event;
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("Cop_y");
    menuitem.icon_name   = "edit-copy";
    menuitem.action_name = "Copy";
    menuitem.activate_cb = terminal_copy_activate_event;
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("_Paste");
    menuitem.icon_name   = "edit-paste";
    menuitem.action_name = "Paste";
    menuitem.activate_cb = terminal_paste_activate_event;
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("Cl_ear scrollback");
    menuitem.action_name = "Clear";
    menuitem.activate_cb = terminal_clear_activate_event;
    w_gtk_menu_item_new (&menuitem);

    /* sep */
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("Preference_s");
    menuitem.icon_name   = "system-run";
    menuitem.action_name = "Preferences";
    menuitem.activate_cb = terminal_preferences_dialog;
    w_gtk_menu_item_new (&menuitem);

    /* sep */
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("Na_me Tab");
    menuitem.icon_name   = "dialog-information";
    menuitem.action_name = "NameTab";
    menuitem.activate_cb = terminal_name_tab_activate_event;
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("Pre_vious Tab");
    menuitem.icon_name   = "go-previous";
    menuitem.action_name = "PreviousTab";
    menuitem.activate_cb = terminal_previous_tab_activate_event;
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("Ne_xt Tab");
    menuitem.icon_name   = "go-next";
    menuitem.action_name = "NextTab";
    menuitem.activate_cb = terminal_next_tab_activate_event;
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("Move Tab _Left");
    menuitem.action_name = "Tabs_MoveTabLeft";
    menuitem.activate_cb = terminal_move_tab_left_activate_event;
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("Move Tab _Right");
    menuitem.action_name = "Tabs_MoveTabRight";
    menuitem.activate_cb = terminal_move_tab_right_activate_event;
    w_gtk_menu_item_new (&menuitem);

    menuitem.label       = _("_Close Tab");
    menuitem.icon_name   = "window-close";
    menuitem.action_name = "CloseTab";
    menuitem.activate_cb = terminal_close_tab_activate_event;
    w_gtk_menu_item_new (&menuitem);

    return menu;
}

