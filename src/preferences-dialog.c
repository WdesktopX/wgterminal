/*
 * This is free and unencumbered software released into the public domain.
 *
 * For more information, please refer to <https://unlicense.org>
 */

// included by preferences.c

#include "w_gtk.h"

static GtkWidget *preferences_dlg;
#define PREFS_SET_OBJECT_ID(id,obj) g_object_set_data(G_OBJECT(preferences_dlg),id,obj)
#define PREFS_GET_OBJECT(id)        g_object_get_data(G_OBJECT(preferences_dlg),id)


static void add2table (GtkWidget *table, GtkWidget *w1, GtkWidget *w2, int *row)
{
    // w1 is usually a GtkLabel, w2 is a combo/entry/button/etc
    if (w1 && w2) {
        // |         label xxxvaluexxxx |
        gtk_grid_attach (GTK_GRID(table), w1, 0, *row, 1, 1);
        gtk_grid_attach (GTK_GRID(table), w2, 1, *row, 1, 1);
#if GTK_MAJOR_VERSION >= 3
        gtk_widget_set_halign (w1, GTK_ALIGN_END);
#else
        // assume w1 is a GtkLabel
        gtk_misc_set_alignment (GTK_MISC(w1), 1.0, 0.5);
#endif
    } else if (w1 && !w2) {
        // widget use both columns
        gtk_grid_attach (GTK_GRID(table), w1, 0, *row, 2, 1);
    }
    *row = *row + 1;
}


/* Initialize and display the preferences dialog. */
void create_preferences_dialog (GtkWidget * parent_window)
{
    GtkWidget *dialog;
    GtkWidget *main_vbox = NULL;
    GtkWidget *hbox, *vbox;
    GtkWidget *notebook;
    //GtkWidget *page[4];
    GtkWidget *table[4];
    GtkWidget *label;
    GtkWidget *checkbox;
    GtkWidget *button;
    GtkWidget *combo;
    GtkWidget *entry;
    char tmp[50];
    int i, x, y;

    dialog = w_gtk_dialog_new (_("LXTerminal"),
                               GTK_WINDOW(parent_window),
                               FALSE,
                               &main_vbox);
    preferences_dlg = dialog;
    gtk_window_set_icon_from_file (GTK_WINDOW(dialog), DATADIR "/icons/hicolor/128x128/apps/lxterminal.png", NULL);

    notebook = gtk_notebook_new ();
    gtk_container_add (GTK_CONTAINER (main_vbox), notebook);
    table[0] = w_gtk_notebook_add_tab (notebook, _("S_tyle"), 14, 2);
    table[1] = w_gtk_notebook_add_tab (notebook, _("_Advanced"), 14, 2);
    table[2] = w_gtk_notebook_add_tab (notebook, _("_Shortcuts"), 14, 2);
    //page[0] = gtk_notebook_get_nth_page (GTK_NOTEBOOK(notebook), 0);

    //==== Tab 0 - Style ====

    i = 0;

    label = gtk_label_new (_("Terminal font"));
    button = gtk_font_button_new ();
    PREFS_SET_OBJECT_ID("terminal_font", button);
    add2table (table[0], label, button, &i);

    label = gtk_label_new (_("Background"));
    button = gtk_color_button_new ();
    PREFS_SET_OBJECT_ID("background_color", button);
    add2table (table[0], label, button, &i);

    label = gtk_label_new (_("Foreground"));
    button = gtk_color_button_new ();
    PREFS_SET_OBJECT_ID("foreground_color", button);
    add2table (table[0], label, button, &i);

    label = gtk_label_new (_("Palette"));
    combo = gtk_combo_box_text_new ();
    PREFS_SET_OBJECT_ID("combobox_color_preset", combo);
    gtk_widget_set_tooltip_text (combo, "Palette preset");
    add2table (table[0], label, combo, &i);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    add2table (table[0], vbox, NULL, &i);
    //-
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start (GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
    static const char *colors[] = {
        N_("Black"), N_("Red"), N_("Green"), N_("Brown"),
        N_("Blue"), N_("Magenta"), N_("Cyan"), N_("Gray"),
        N_("Darkgray"), N_("Bright Red"), N_("Bright Green"), N_("Yellow"),
        N_("Bright Blue"), N_("Bright Magenta"), N_("Bright Cyan"), N_("White"), NULL
    };
    y = 0;
    for (x = 0; x < 8; x++)
    {
        button = gtk_color_button_new ();
        gtk_box_pack_start (GTK_BOX(hbox), button, TRUE, TRUE, 0);
        snprintf (tmp, sizeof(tmp), "color_%d", y);
        gtk_widget_set_tooltip_text (button, colors[y]);
        PREFS_SET_OBJECT_ID(tmp, button); /* color_x */
        //puts (tmp);
        y++;
    }
    //-
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start (GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
    for (x = 0; x < 8; x++)
    {
        button = gtk_color_button_new ();
        gtk_box_pack_start (GTK_BOX(hbox), button, TRUE, TRUE, 0);
        snprintf (tmp, sizeof(tmp), "color_%d", y);
        gtk_widget_set_tooltip_text (button, colors[y]);
        PREFS_SET_OBJECT_ID(tmp, button); /* color_x */
        //puts (tmp);
        y++;
    }

    label = gtk_label_new (_("Allow bold font"));
    checkbox = gtk_check_button_new ();
    PREFS_SET_OBJECT_ID("allow_bold", checkbox);
    add2table (table[0], label, checkbox, &i);

    label = gtk_label_new (_("Bold is bright"));
    checkbox = gtk_check_button_new ();
    PREFS_SET_OBJECT_ID("label_bold_bright", label);
    PREFS_SET_OBJECT_ID("bold_bright", checkbox);
    add2table (table[0], label, checkbox, &i);

    label = gtk_label_new (_("Cursor blink"));
    checkbox = gtk_check_button_new ();
    PREFS_SET_OBJECT_ID("cursor_blink", checkbox);
    add2table (table[0], label, checkbox, &i);

    label = gtk_label_new (_("Cursor style"));
    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    add2table (table[0], label, vbox, &i);
    //-
    button = gtk_radio_button_new_with_mnemonic (NULL, _("Block"));
    gtk_box_pack_start (GTK_BOX(vbox), button, FALSE, FALSE, 0);
    PREFS_SET_OBJECT_ID("cursor_style_block", button);
    //-
    button = gtk_radio_button_new_with_mnemonic (
                gtk_radio_button_get_group (GTK_RADIO_BUTTON(button)),
                _("Underline"));
    gtk_box_pack_start (GTK_BOX(vbox), button, FALSE, FALSE, 0);
    PREFS_SET_OBJECT_ID("cursor_style_underline", button);

    label = gtk_label_new (_("Audible bell"));
    checkbox = gtk_check_button_new ();
    PREFS_SET_OBJECT_ID("audible_bell", checkbox);
    add2table (table[0], label, checkbox, &i);

    label = gtk_label_new (_("Visual bell"));
    checkbox = gtk_check_button_new ();
    PREFS_SET_OBJECT_ID("visual_bell", checkbox);
    add2table (table[0], label, checkbox, &i);

    //==== Tab 1 - Advanced ====

    i = 0;

    label = gtk_label_new (_("Tab panel position"));
    combo = gtk_combo_box_text_new ();
    add2table (table[1], label, combo, &i);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo), _("Top"));
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo), _("Bottom"));
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo), _("Left"));
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo), _("Right"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(combo), 0);
    PREFS_SET_OBJECT_ID("tab_position", combo);

    label = gtk_label_new (_("Scrollback lines"));
    entry = gtk_spin_button_new_with_range (0, 100000, 10);
    add2table (table[1], label, entry, &i);
    PREFS_SET_OBJECT_ID("scrollback_lines", entry);
    gtk_entry_set_width_chars (GTK_ENTRY(entry), 6);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(entry), 1000);

    label = gtk_label_new (_("Default window size"));
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
    add2table (table[1], label, hbox, &i);
    //-
    entry = gtk_spin_button_new_with_range (0, 1000, 1);
    gtk_entry_set_width_chars (GTK_ENTRY(entry), 4);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(entry), 80);
    PREFS_SET_OBJECT_ID("geometry_columns", entry);
    gtk_box_pack_start (GTK_BOX(hbox), entry, FALSE, FALSE, 0);
    //-
    label = gtk_label_new ("x");
    gtk_box_pack_start (GTK_BOX(hbox), label, FALSE, FALSE, 0);
    //-
    entry = gtk_spin_button_new_with_range (0, 1000, 1);
    gtk_entry_set_width_chars (GTK_ENTRY(entry), 4);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(entry), 24);
    PREFS_SET_OBJECT_ID("geometry_rows", entry);
    gtk_box_pack_start (GTK_BOX(hbox), entry, FALSE, FALSE, 0);

    label = gtk_label_new (_("Hide scroll bar"));
    checkbox = gtk_check_button_new ();
    add2table (table[1], label, checkbox, &i);
    PREFS_SET_OBJECT_ID("hide_scroll_bar", checkbox);

    label = gtk_label_new (_("Hide menu bar"));
    checkbox = gtk_check_button_new ();
    add2table (table[1], label, checkbox, &i);
    PREFS_SET_OBJECT_ID("hide_menu_bar", checkbox);

    label = gtk_label_new (_("Hide Close buttons"));
    checkbox = gtk_check_button_new ();
    add2table (table[1], label, checkbox, &i);
    PREFS_SET_OBJECT_ID("hide_close_button", checkbox);

    label = gtk_label_new (_("Hide mouse pointer"));
    checkbox = gtk_check_button_new ();
    add2table (table[1], label, checkbox, &i);
    PREFS_SET_OBJECT_ID("hide_pointer", checkbox);

    label = gtk_label_new ("");
    add2table (table[1], label, NULL, &i);
    
    label = gtk_label_new (_("Select-by-word characters"));
    entry = gtk_entry_new ();
    add2table (table[1], label, entry, &i);
    PREFS_SET_OBJECT_ID("select_by_word", entry);

    label = gtk_label_new (_("Disable menu shortcut key (F10 by default)"));
    checkbox = gtk_check_button_new ();
    add2table (table[1], label, checkbox, &i);
    PREFS_SET_OBJECT_ID("disable_f10", checkbox);

    label = gtk_label_new (_("Disable using Alt-n for tabs and menu"));
    checkbox = gtk_check_button_new ();
    add2table (table[1], label, checkbox, &i);
    PREFS_SET_OBJECT_ID("disable_alt", checkbox);

    label = gtk_label_new (_("Disable confirmation before closing a window with multiple tabs"));
    checkbox = gtk_check_button_new ();
    add2table (table[1], label, checkbox, &i);
    PREFS_SET_OBJECT_ID("disable_confirm", checkbox);

    label = gtk_label_new (_("Tab width"));
    entry = gtk_spin_button_new_with_range (0, 1000, 1);
    add2table (table[1], label, entry, &i);
    PREFS_SET_OBJECT_ID("tab_width", entry);
    gtk_entry_set_width_chars (GTK_ENTRY(entry), 4);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(entry), 100);


    //==== Tab 2 - Shortcuts ====

    static const char *accel_object_tag[] = {
        NEW_WINDOW_ACCEL,
        NEW_TAB_ACCEL,
        CLOSE_TAB_ACCEL,
        CLOSE_WINDOW_ACCEL,
        COPY_ACCEL,
        PASTE_ACCEL,
        NAME_TAB_ACCEL,
        PREVIOUS_TAB_ACCEL,
        NEXT_TAB_ACCEL,
        MOVE_TAB_LEFT_ACCEL,
        MOVE_TAB_RIGHT_ACCEL,
        ZOOM_IN_ACCEL,
        ZOOM_OUT_ACCEL,
        ZOOM_RESET_ACCEL,
        NULL,
    };

    static const char *accel_label[] = {
        N_("New Window"),
        N_("New Tab"),
        N_("Close Tab"),
        N_("Close Window"),
        N_("Copy"),
        N_("Paste"),
        N_("Name Tab"),
        N_("Previous Tab"),
        N_("Next Tab"),
        N_("Move Tab Left"),
        N_("Move Tab Right"),
        N_("Zoom In"),
        N_("Zoom Out"),
        N_("Zoom Reset"),
        NULL,
    };

    i = 0;
    for (x = 0; accel_label[x]; x++)
    {
        label = gtk_label_new (gettext(accel_label[x]));
        entry = gtk_entry_new ();
        PREFS_SET_OBJECT_ID(accel_object_tag[x], entry);
        add2table (table[2], label, entry, &i);
    }

    //====
    gtk_dialog_add_button (GTK_DIALOG(dialog), "gtk-ok", GTK_RESPONSE_OK);
    gtk_dialog_add_button (GTK_DIALOG(dialog), "gtk-cancel", GTK_RESPONSE_CANCEL);
    
    gtk_widget_show_all (main_vbox);
}
