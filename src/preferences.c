/**
 *      Copyright 2008 Fred Chien <cfsghost@gmail.com>
 *      Copyright (c) 2010 LxDE Developers, see the file AUTHORS for details.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#include "common.h"

gint preset_custom_id;
GtkWidget *preferences_dlg;
#define PREFS_SET_OBJECT_ID(id,obj) g_object_set_data(G_OBJECT(preferences_dlg),id,obj)
#define PREFS_GET_OBJECT(id)        g_object_get_data(G_OBJECT(preferences_dlg),id)

// =======================================================================
//                    PREFERENCES DIALOG 
// =======================================================================

static void add2table (GtkWidget *table, GtkWidget *w1, GtkWidget *w2, int *row)
{
    if (!w1 && !w2) {
        w1 = gtk_label_new ("");
    }
    if (w1 && w2) {
        // w1 is a GtkLabel, w2 is a combo/entry/button/etc
        // |         label xxxvaluexxxx |
        gtk_grid_attach (GTK_GRID(table), w1, 0, *row, 1, 1);
        gtk_grid_attach (GTK_GRID(table), w2, 1, *row, 1, 1);
        gtk_widget_set_halign (w1, GTK_ALIGN_END);
    } else if (w1 && !w2) {
        // widget use both columns
        gtk_grid_attach (GTK_GRID(table), w1, 0, *row, 2, 1);
    } else if (w2) {
        // only 2nd column
        gtk_grid_attach (GTK_GRID(table), w2, 1, *row, 1, 1);
    }
    *row = *row + 1;
}


static void create_preferences_dialog (GtkWidget * parent_window)
{
    /* Initialize dialog */
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

    add2table (table[0], NULL, NULL, &i);

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
        gtk_widget_set_tooltip_text (button, gettext(colors[y]));
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

    add2table (table[0], NULL, NULL, &i);

    checkbox = gtk_check_button_new_with_label (_("Bold is bright"));
    PREFS_SET_OBJECT_ID("bold_bright", checkbox);
    add2table (table[0], NULL, checkbox, &i);

    checkbox = gtk_check_button_new_with_label (_("Cursor blink"));
    PREFS_SET_OBJECT_ID("cursor_blink", checkbox);
    add2table (table[0], NULL, checkbox, &i);

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

    checkbox = gtk_check_button_new_with_label (_("Audible bell"));
    PREFS_SET_OBJECT_ID("audible_bell", checkbox);
    add2table (table[0], NULL, checkbox, &i);;

    checkbox = gtk_check_button_new_with_label (_("Visual bell"));
    PREFS_SET_OBJECT_ID("visual_bell", checkbox);
    add2table (table[0], NULL, checkbox, &i);

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

    add2table (table[1], NULL, NULL, &i);

    label = gtk_label_new (_("Select-by-word characters"));
    entry = gtk_entry_new ();
    add2table (table[1], label, entry, &i);
    PREFS_SET_OBJECT_ID("select_by_word", entry);

    label = gtk_label_new (_("Tab width"));
    entry = gtk_spin_button_new_with_range (0, 1000, 1);
    add2table (table[1], label, entry, &i);
    PREFS_SET_OBJECT_ID("tab_width", entry);
    gtk_entry_set_width_chars (GTK_ENTRY(entry), 4);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(entry), 100);

    add2table (table[1], NULL, NULL, &i);

    checkbox = gtk_check_button_new_with_label (_("Hide scroll bar"));
    add2table (table[1], NULL, checkbox, &i);
    PREFS_SET_OBJECT_ID("hide_scroll_bar", checkbox);

    checkbox = gtk_check_button_new_with_label (_("Hide menu bar"));
    add2table (table[1], NULL, checkbox, &i);
    PREFS_SET_OBJECT_ID("hide_menu_bar", checkbox);

    checkbox = gtk_check_button_new_with_label (_("Hide Close buttons"));
    add2table (table[1], NULL, checkbox, &i);
    PREFS_SET_OBJECT_ID("hide_close_button", checkbox);

    checkbox = gtk_check_button_new_with_label (_("Hide mouse pointer"));
    add2table (table[1], NULL, checkbox, &i);
    PREFS_SET_OBJECT_ID("hide_pointer", checkbox);

    checkbox = gtk_check_button_new_with_label (_("Disable menu shortcut key (F10 by default)"));
    add2table (table[1], NULL, checkbox, &i);
    PREFS_SET_OBJECT_ID("disable_f10", checkbox);

    checkbox = gtk_check_button_new_with_label (_("Disable using Alt-n for tabs and menu"));
    add2table (table[1], NULL, checkbox, &i);
    PREFS_SET_OBJECT_ID("disable_alt", checkbox);

    checkbox = gtk_check_button_new_with_label (_("Disable confirmation before closing a window with multiple tabs"));
    add2table (table[1], NULL, checkbox, &i);
    PREFS_SET_OBJECT_ID("disable_confirm", checkbox);


    //==== Tab 2 - Shortcuts ====

    static const char *accel_strv[] =
    {  // object tag            label
        NEW_WINDOW_ACCEL,    N_("New Window"),
        NEW_TAB_ACCEL,       N_("New Tab"),
        CLOSE_TAB_ACCEL,     N_("Close Tab"),
        CLOSE_WINDOW_ACCEL,  N_("Close Window"),
        COPY_ACCEL,          N_("Copy"),
        PASTE_ACCEL,         N_("Paste"),
        NAME_TAB_ACCEL,      N_("Name Tab"),
        PREVIOUS_TAB_ACCEL,  N_("Previous Tab"),
        NEXT_TAB_ACCEL,      N_("Next Tab"),
        MOVE_TAB_LEFT_ACCEL, N_("Move Tab Left"),
        MOVE_TAB_RIGHT_ACCEL,N_("Move Tab Right"),
        ZOOM_IN_ACCEL,       N_("Zoom In"),
        ZOOM_OUT_ACCEL,      N_("Zoom Out"),
        ZOOM_RESET_ACCEL,    N_("Zoom Reset"),
        NULL, NULL,
    };

    i = 0;
    for (x = 0; accel_strv[x]; x++)
    {
        label = gtk_label_new (gettext(accel_strv[x+1]));
        entry = gtk_entry_new ();
        PREFS_SET_OBJECT_ID(accel_strv[x], entry);
        add2table (table[2], label, entry, &i);
        x++;
    }

    //====
    gtk_dialog_add_button (GTK_DIALOG(dialog), "gtk-ok", GTK_RESPONSE_OK);
    gtk_dialog_add_button (GTK_DIALOG(dialog), "gtk-cancel", GTK_RESPONSE_CANCEL);

    gtk_widget_show_all (main_vbox);
}


// =======================================================================
//                             CALLBACKS
// =======================================================================

/* Handler for "font-set" signal on Terminal Font font button. */
static void preferences_dialog_font_set_event(GtkFontButton * widget, Setting * setting)
{
    g_free(setting->font_name);
#if GTK_CHECK_VERSION (3, 2, 0)
    setting->font_name = gtk_font_chooser_get_font(GTK_FONT_CHOOSER(widget));
#else
    setting->font_name = g_strdup(gtk_font_button_get_font_name(widget));
#endif
    setting->geometry_change = TRUE;        /* Force the terminals to resize */
}

/* Handler for "color-set" signal on Background Color color button. */
static gboolean preferences_dialog_background_color_set_event(GtkWidget * widget, Setting * setting)
{
#if VTE_CHECK_VERSION (0, 38, 0)
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(widget), &setting->background_color);
#else
    gtk_color_button_get_color(GTK_COLOR_BUTTON(widget), &setting->background_color);
    setting->background_alpha = gtk_color_button_get_alpha(GTK_COLOR_BUTTON(widget));

    if (setting->background_alpha == 0)
    {
        setting->background_alpha = 1;
    }
#endif

    GtkWidget *combo = PREFS_GET_OBJECT("combobox_color_preset");
    gtk_combo_box_set_active (GTK_COMBO_BOX(combo), preset_custom_id);
    setting->color_preset = color_presets[preset_custom_id].name;
    return FALSE;
}

/* Handler for "color-set" signal on Foreground Color color button. */
static gboolean preferences_dialog_foreground_color_set_event(GtkWidget * widget, Setting * setting)
{
#if VTE_CHECK_VERSION (0, 38, 0)
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(widget), &setting->foreground_color);
#else
    gtk_color_button_get_color(GTK_COLOR_BUTTON(widget), &setting->foreground_color);
#endif

    GtkWidget *combo = PREFS_GET_OBJECT("combobox_color_preset");
    gtk_combo_box_set_active (GTK_COMBO_BOX(combo), preset_custom_id);
    setting->color_preset = color_presets[preset_custom_id].name;
    return FALSE;
}

/* Handler for "color-set" signal on any palette color button. */
static gboolean preferences_dialog_palette_color_set_event(GtkColorButton * widget, Setting * setting)
{
    int i;
    GtkWidget *w;
    char color_x[50];

    for (i=0;i<16;i++)
    {
        snprintf (color_x, sizeof(color_x), "color_%i", i);
        w = PREFS_GET_OBJECT(color_x);
#if VTE_CHECK_VERSION (0, 38, 0)
        gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w), &setting->palette_color[i]);
#else
        gtk_color_button_get_color(GTK_COLOR_BUTTON(w), &setting->palette_color[i]);
#endif
    }

    w = PREFS_GET_OBJECT("combobox_color_preset");
    gtk_combo_box_set_active (GTK_COMBO_BOX(w), preset_custom_id);
    setting->color_preset = color_presets[preset_custom_id].name;
    return FALSE;
}

/* Handler for "changed" signal on palette preset menu */
static gboolean preferences_dialog_palette_preset_changed_event(GtkComboBox * widget, Setting * setting)
{
    gint active;
    int i;
    GtkWidget *w;

    active = gtk_combo_box_get_active(widget);
    if (g_strcmp0(color_presets[active].name, "Custom") == 0) {
        return FALSE;
    }

    setting->color_preset = color_presets[active].name;

    w = PREFS_GET_OBJECT("background_color");
#if VTE_CHECK_VERSION (0, 38, 0)
    gdk_rgba_parse(&setting->background_color, color_presets[active].background_color);
    gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(w), &setting->background_color);
#else
    gdk_color_parse(color_presets[active].background_color, &setting->background_color);
    setting->background_alpha = 65535;
    gtk_color_button_set_color(GTK_COLOR_BUTTON(w), &setting->background_color);
    gtk_color_button_set_alpha(GTK_COLOR_BUTTON(w), 65535);
#endif

    w = PREFS_GET_OBJECT("foreground_color");
#if VTE_CHECK_VERSION (0, 38, 0)
    gdk_rgba_parse(&setting->foreground_color, color_presets[active].foreground_color);
    gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(w), &setting->foreground_color);
#else
    gdk_color_parse(color_presets[active].foreground_color, &setting->foreground_color);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(w), &setting->foreground_color);
#endif

    for (i=0;i<16;i++) {
        gchar *object_key = g_strdup_printf("color_%i", i);
        w = PREFS_GET_OBJECT(object_key);
#if VTE_CHECK_VERSION (0, 38, 0)
        gdk_rgba_parse(&setting->palette_color[i], color_presets[active].palette[i]);
        gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(w), &setting->palette_color[i]);
#else
        gdk_color_parse(color_presets[active].palette[i], &setting->palette_color[i]);
        gtk_color_button_set_color(GTK_COLOR_BUTTON(w), &setting->palette_color[i]);
#endif
	g_free(object_key);
    }

    return FALSE;
}

/* Handler for "changed" signal on Tab Position combo box. */
static void preferences_dialog_tab_position_changed_event(GtkComboBox * widget, Setting * setting)
{
    /* Convert the index into a string, which is what we put in the configuration file. */
    const char * p = NULL;
    switch (gtk_combo_box_get_active(widget))
    {
        case 0: p = "top";    break;
        case 1: p = "bottom"; break;
        case 2: p = "left";   break;
        case 3: p = "right";  break;
    }
    if (p != NULL)
    {
        g_free(setting->tab_position);
        setting->tab_position = g_strdup(p);
    }
    //terminal_settings_apply_to_all(terminal);
}

/* Handler for "value-changed" signal on spin button. */
static void preferences_dialog_int_value_changed_event(GtkSpinButton * widget, gint * value)
{
    *value = gtk_spin_button_get_value_as_int(widget);
}

/* Convert the user preference on tab position, expressed as a string, to the internal representation.
 * These have to match the order in the .glade file. */
gint terminal_tab_get_position_id(gchar * position)
{
    if (strcmp(position, "bottom") == 0)     return 1;
    else if (strcmp(position, "left") == 0)  return 2;
    else if (strcmp(position, "right") == 0) return 3;
    else                                     return 0;
}

/* Generic "toggled" handler for GtkToggleButton events */
static void preferences_dialog_generic_toggled_event(GtkToggleButton * widget, gboolean * s)
{
    *s = gtk_toggle_button_get_active(widget);
}

/* Generic "focus-out-event" handler for GtkEntry events */
static gboolean preferences_dialog_generic_focus_out_event(GtkWidget * widget, GdkEventFocus * event, gchar ** s)
{
    g_free(*s);
    *s = g_strdup(gtk_entry_get_text(GTK_ENTRY(widget)));
    return FALSE;
}

/* Specific "key-press-event" handler for shortcut GtkEntry events */
static gboolean preferences_dialog_shortcut_key_press_event(GtkWidget * widget, GdkEventKey * ekey, gchar ** s)
{
    guint key = ekey->keyval;
    guint mod = ekey->state & gtk_accelerator_get_default_mod_mask();
    gchar * lbl;
    GList * sib;

    /* Prevent Tab being used as one of the keys, and return false if doing so. */
    if (key == GDK_KEY(Tab) || key == GDK_KEY(ISO_Left_Tab)) {
        return FALSE;
    }

    lbl = gtk_accelerator_get_label(key, mod);

    /* Look for duplicate accelerator. */
    for(sib = gtk_container_get_children(GTK_CONTAINER(gtk_widget_get_parent(widget))); sib; sib = sib->next) {
	if(GTK_IS_ENTRY(sib->data) && GTK_WIDGET(sib->data) != widget && !g_strcmp0(lbl, gtk_entry_get_text(GTK_ENTRY(sib->data)))) {
            goto free_lbl;
	}
    }

    /* Make sure accelerator is valid. */
    if (!gtk_accelerator_valid(key, mod)) {
        goto free_lbl;
    }

    g_free(*s);
    *s = gtk_accelerator_name(key, mod);
    gtk_entry_set_text(GTK_ENTRY(widget), lbl);

free_lbl:
    g_free(lbl);
    return TRUE;
}

static void accel_set_label(const gchar * name, GtkWidget * w)
{
    guint key;
    GdkModifierType mods;
    gchar * label;
    gtk_accelerator_parse(name, &key, &mods);
    label = gtk_accelerator_get_label(key, mods);
    gtk_entry_set_text(GTK_ENTRY(w), label);
    g_free(label);
}


// =======================================================================
//         CREATE DIALOG, ASSIGN VALUES AND SIGNALS
// =======================================================================

/* Initialize and display the preferences dialog. */
void terminal_preferences_dialog(GtkMenuItem * action, LXTerminal * terminal)
{
    int i;
    Setting * setting = copy_setting(get_setting());

    create_preferences_dialog (terminal->window);

    GtkWidget * w = PREFS_GET_OBJECT("terminal_font");
#if GTK_CHECK_VERSION (3, 2, 0)
    gtk_font_chooser_set_font(GTK_FONT_CHOOSER(w), setting->font_name);
#else
    gtk_font_button_set_font_name(GTK_FONT_BUTTON(w), setting->font_name);
#endif
    g_signal_connect(G_OBJECT(w), "font-set", G_CALLBACK(preferences_dialog_font_set_event), setting);

    w = PREFS_GET_OBJECT("background_color");
#if VTE_CHECK_VERSION (0, 38, 0)
    gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(w), &setting->background_color);
#else
    gtk_color_button_set_color(GTK_COLOR_BUTTON(w), &setting->background_color);
    gtk_color_button_set_alpha(GTK_COLOR_BUTTON(w), setting->background_alpha);
#endif
    g_signal_connect(G_OBJECT(w), "color-set", 
        G_CALLBACK(preferences_dialog_background_color_set_event), setting);

    w = PREFS_GET_OBJECT("foreground_color");
#if VTE_CHECK_VERSION (0, 38, 0)
    gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(w), &setting->foreground_color);
#else
    gtk_color_button_set_color(GTK_COLOR_BUTTON(w), &setting->foreground_color);
#endif
    g_signal_connect(G_OBJECT(w), "color-set", 
        G_CALLBACK(preferences_dialog_foreground_color_set_event), setting);

    for (i=0; i<16; i++) {
        gchar *object_key = g_strdup_printf("color_%i", i);
        w = PREFS_GET_OBJECT(object_key);
#if VTE_CHECK_VERSION (0, 38, 0)
        gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(w), &setting->palette_color[i]);
#else
        gtk_color_button_set_color(GTK_COLOR_BUTTON(w), &setting->palette_color[i]);
#endif
        g_signal_connect(G_OBJECT(w), "color-set",
            G_CALLBACK(preferences_dialog_palette_color_set_event), setting);
	g_free(object_key);
    }

    GtkWidget *combo = PREFS_GET_OBJECT("combobox_color_preset");
    gboolean preset_is_set = FALSE;
    for (i=0; ; i++)
    {
        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo), color_presets[i].name);

        if (g_strcmp0(color_presets[i].name, setting->color_preset) == 0) {
            gtk_combo_box_set_active (GTK_COMBO_BOX(combo), i);
            preset_is_set = TRUE;
        }

        if (g_strcmp0(color_presets[i].name, "Custom") == 0) {
            if (preset_is_set == FALSE) {
                gtk_combo_box_set_active (GTK_COMBO_BOX(combo), i);
            }
            preset_custom_id = i;
            break;
        }
    }

    g_signal_connect(G_OBJECT(combo), "changed",
            G_CALLBACK(preferences_dialog_palette_preset_changed_event), setting);

    w = PREFS_GET_OBJECT("bold_bright");
#if VTE_CHECK_VERSION (0, 52, 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), setting->bold_bright);
    g_signal_connect(G_OBJECT(w), "toggled", 
        G_CALLBACK(preferences_dialog_generic_toggled_event), &setting->bold_bright);
#else
    gtk_widget_hide(w);
#endif

    w = PREFS_GET_OBJECT("cursor_blink");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), setting->cursor_blink);
    g_signal_connect(G_OBJECT(w), "toggled", 
        G_CALLBACK(preferences_dialog_generic_toggled_event), &setting->cursor_blink);

    w = PREFS_GET_OBJECT("cursor_style_block");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), ! setting->cursor_underline);

    w = PREFS_GET_OBJECT("cursor_style_underline");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), setting->cursor_underline);
    g_signal_connect(G_OBJECT(w), "toggled", 
        G_CALLBACK(preferences_dialog_generic_toggled_event), &setting->cursor_underline);

    w = PREFS_GET_OBJECT("audible_bell");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), setting->audible_bell);
    g_signal_connect(G_OBJECT(w), "toggled", 
        G_CALLBACK(preferences_dialog_generic_toggled_event), &setting->audible_bell);

    w = PREFS_GET_OBJECT("visual_bell");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), setting->visual_bell);
    g_signal_connect(G_OBJECT(w), "toggled", 
        G_CALLBACK(preferences_dialog_generic_toggled_event), &setting->visual_bell);

    w = PREFS_GET_OBJECT("tab_position");
    gtk_combo_box_set_active(GTK_COMBO_BOX(w), terminal_tab_get_position_id(setting->tab_position));
    g_signal_connect(G_OBJECT(w), "changed", 
        G_CALLBACK(preferences_dialog_tab_position_changed_event), setting);

    w = PREFS_GET_OBJECT("scrollback_lines");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), setting->scrollback);
    g_signal_connect(G_OBJECT(w), "value-changed", 
        G_CALLBACK(preferences_dialog_int_value_changed_event), &setting->scrollback);

    w = PREFS_GET_OBJECT("geometry_columns");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), setting->geometry_columns);
    g_signal_connect(G_OBJECT(w), "value-changed", 
        G_CALLBACK(preferences_dialog_int_value_changed_event), &setting->geometry_columns);

    w = PREFS_GET_OBJECT("geometry_rows");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), setting->geometry_rows);
    g_signal_connect(G_OBJECT(w), "value-changed", 
        G_CALLBACK(preferences_dialog_int_value_changed_event), &setting->geometry_rows);

    w = PREFS_GET_OBJECT("hide_scroll_bar");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), setting->hide_scroll_bar);
    g_signal_connect(G_OBJECT(w), "toggled", 
        G_CALLBACK(preferences_dialog_generic_toggled_event), &setting->hide_scroll_bar);

    w = PREFS_GET_OBJECT("hide_menu_bar");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), setting->hide_menu_bar);
    g_signal_connect(G_OBJECT(w), "toggled", 
        G_CALLBACK(preferences_dialog_generic_toggled_event), &setting->hide_menu_bar);

    w = PREFS_GET_OBJECT("hide_close_button");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), setting->hide_close_button);
    g_signal_connect(G_OBJECT(w), "toggled", 
        G_CALLBACK(preferences_dialog_generic_toggled_event), &setting->hide_close_button);

    w = PREFS_GET_OBJECT("hide_pointer");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), setting->hide_pointer);
    g_signal_connect(G_OBJECT(w), "toggled", 
        G_CALLBACK(preferences_dialog_generic_toggled_event), &setting->hide_pointer);

    w = PREFS_GET_OBJECT("select_by_word");
    gtk_entry_set_text(GTK_ENTRY(w), setting->word_selection_characters);
    g_signal_connect(G_OBJECT(w), "focus-out-event", 
        G_CALLBACK(preferences_dialog_generic_focus_out_event), &setting->word_selection_characters);

    w = PREFS_GET_OBJECT("disable_f10");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), setting->disable_f10);
    g_signal_connect(G_OBJECT(w), "toggled", 
        G_CALLBACK(preferences_dialog_generic_toggled_event), &setting->disable_f10);

    w = PREFS_GET_OBJECT("disable_alt");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), setting->disable_alt);
    g_signal_connect(G_OBJECT(w), "toggled", 
        G_CALLBACK(preferences_dialog_generic_toggled_event), &setting->disable_alt);
    
    w = PREFS_GET_OBJECT("disable_confirm");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), setting->disable_confirm);
    g_signal_connect(G_OBJECT(w), "toggled", 
        G_CALLBACK(preferences_dialog_generic_toggled_event), &setting->disable_confirm);

    w = PREFS_GET_OBJECT("tab_width");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), setting->tab_width);
    g_signal_connect(G_OBJECT(w), "value-changed", 
        G_CALLBACK(preferences_dialog_int_value_changed_event), &setting->tab_width);

    /* Shortcuts */
#define PREF_SETUP_SHORTCUT(OBJ, VAR) \
    w = PREFS_GET_OBJECT(OBJ); \
    accel_set_label(VAR, w); \
    g_signal_connect(G_OBJECT(w), "key-press-event", \
        G_CALLBACK(preferences_dialog_shortcut_key_press_event), &VAR); \

    PREF_SETUP_SHORTCUT(NEW_WINDOW_ACCEL, setting->new_window_accel)
    PREF_SETUP_SHORTCUT(NEW_TAB_ACCEL, setting->new_tab_accel)
    PREF_SETUP_SHORTCUT(CLOSE_TAB_ACCEL, setting->close_tab_accel)
    PREF_SETUP_SHORTCUT(CLOSE_WINDOW_ACCEL, setting->close_window_accel)
    PREF_SETUP_SHORTCUT(COPY_ACCEL, setting->copy_accel)
    PREF_SETUP_SHORTCUT(PASTE_ACCEL, setting->paste_accel)
    PREF_SETUP_SHORTCUT(NAME_TAB_ACCEL, setting->name_tab_accel)
    PREF_SETUP_SHORTCUT(PREVIOUS_TAB_ACCEL, setting->previous_tab_accel)
    PREF_SETUP_SHORTCUT(NEXT_TAB_ACCEL, setting->next_tab_accel)
    PREF_SETUP_SHORTCUT(MOVE_TAB_LEFT_ACCEL, setting->move_tab_left_accel)
    PREF_SETUP_SHORTCUT(MOVE_TAB_RIGHT_ACCEL, setting->move_tab_right_accel)
    PREF_SETUP_SHORTCUT(ZOOM_IN_ACCEL, setting->zoom_in_accel)
    PREF_SETUP_SHORTCUT(ZOOM_OUT_ACCEL, setting->zoom_out_accel)
    PREF_SETUP_SHORTCUT(ZOOM_RESET_ACCEL, setting->zoom_reset_accel)

    int result = gtk_dialog_run (GTK_DIALOG(preferences_dlg));
    /* Dismiss dialog. */
    gtk_widget_destroy(GTK_WIDGET(preferences_dlg));
    if (result == GTK_RESPONSE_OK)
    {
        set_setting(setting);
        save_setting();
        terminal_settings_apply_to_all(terminal);
    }
    else
    {
        free_setting(&setting);
    }
}
