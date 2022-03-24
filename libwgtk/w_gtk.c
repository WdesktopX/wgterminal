/*
 * This is free and unencumbered software released into the public domain.
 *
 * For more information, please refer to <https://unlicense.org>
 */

#if defined(__clang__)
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "w_gtk.h"
#include <stdio.h>
#include <string.h>

GtkWidget * w_gtk_window_new (const char * title,
                              GtkWindow * parent,
                              GtkApplication * app, // gtkcompat.h < 3 = `void * app`
                              gboolean resizable)
{
    GtkWidget * window;
    if (app) {
#if GTK_CHECK_VERSION (3, 4, 0)
        window = gtk_application_window_new (app);
#else
        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
#endif
    } else {
        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    }
    gtk_window_set_title (GTK_WINDOW (window), title);
    gtk_container_set_border_width (GTK_CONTAINER (window), 4); /* padding */
    if (parent) {
        gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (parent));
        gtk_window_set_modal (GTK_WINDOW (window), TRUE);
        gtk_window_set_destroy_with_parent (GTK_WINDOW (window), TRUE);
        gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ON_PARENT);
        gtk_window_set_skip_pager_hint (GTK_WINDOW (window), TRUE);
        gtk_window_set_skip_taskbar_hint (GTK_WINDOW (window), TRUE);
    }
    if (!resizable) {
        // no need to call this if TRUE, unexpected behavior in GTK3 IIRC
        gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
    }

    return window;
}


GtkWidget * w_gtk_dialog_new (const char * title,
                              GtkWindow * parent,
                              gboolean resizable,
                              GtkWidget ** main_vbox) /* out */
{
    GtkWidget * dialog;
    dialog = gtk_dialog_new ();
    gtk_window_set_title (GTK_WINDOW (dialog), title);
    gtk_container_set_border_width (GTK_CONTAINER (dialog), 4); /* padding */
    if (parent) {
        gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (parent));
        gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
        gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);
        gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ON_PARENT);
        gtk_window_set_skip_pager_hint (GTK_WINDOW (dialog), TRUE);
        gtk_window_set_skip_taskbar_hint (GTK_WINDOW (dialog), TRUE);
    }
#if GTK_MAJOR_VERSION >= 3 // always need a parent window
    else {
        GtkWidget * parent = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (parent));
        gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);
    }
#endif
    if (!resizable) {
        // no need to call this if TRUE, unexpected behavior in GTK3 IIRC
        gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
    }

    if (main_vbox) {
        *main_vbox = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
        /* padding */
        gtk_container_set_border_width (GTK_CONTAINER (*main_vbox), 4);
    }

    return dialog;
}


GtkWidget * w_gtk_frame_vbox_new (char * label,
                                  GtkWidget * parent_box,
                                  int children_spacing,
                                  int box_padding)
{
    // returns a vbox inside a frame
    GtkWidget * frame = gtk_frame_new (label);
    gtk_box_pack_start (GTK_BOX (parent_box), frame, FALSE, FALSE, 0);

    GtkWidget * frame_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, children_spacing);
    gtk_container_add (GTK_CONTAINER (frame), frame_vbox);
    if (box_padding) {
        /* padding */
        gtk_container_set_border_width (GTK_CONTAINER (frame_vbox), box_padding);
    }
    return frame_vbox;
}


GtkWidget * w_gtk_image_new_from_icon_name (const char *icon_name, GtkIconSize size)
{
    GtkWidget *img = NULL;
    GtkIconTheme *icon_theme = gtk_icon_theme_get_default ();
    if (gtk_icon_theme_has_icon (icon_theme, icon_name)) {
        img = gtk_image_new_from_icon_name (icon_name, size);
    } else if (strncmp (icon_name, "gtk-", 4) == 0) {
        img = gtk_image_new_from_stock (icon_name, size);
    } else {
        // get blank/invalid image
        fprintf (stderr, "%s was not found in icon theme", icon_name);
        img = gtk_image_new_from_icon_name (icon_name, size);
    }
    return img;
}


void w_gtk_image_set_from_icon_name (GtkImage *img, const char *icon_name, GtkIconSize size)
{
    if (!img) {
        return;
    }
    GtkIconTheme *icon_theme = gtk_icon_theme_get_default ();
    if (gtk_icon_theme_has_icon (icon_theme, icon_name)) {
        gtk_image_set_from_icon_name (img, icon_name, size);
    } else if (strncmp (icon_name, "gtk-", 4) == 0) {
        gtk_image_set_from_stock (img, icon_name, size);
    } else {
        // set blank/invalid image
        fprintf (stderr, "%s was not found in icon theme", icon_name);
        gtk_image_set_from_icon_name (img, icon_name, size);
    }
}


void w_gtk_button_set_icon_name (GtkButton *button, const char *icon_name)
{
    GtkWidget *img;
    img = w_gtk_image_new_from_icon_name (icon_name, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image (button, img);
}


GtkWidget * w_gtk_button_new (const char * label,
                              const char * icon_name,
                              gpointer clicked_cb,
                              gpointer cdata)
{
    GtkWidget * button;
    if (label) {
        button = gtk_button_new_with_mnemonic (label);
    } else {
        button = gtk_button_new ();
    }
    if (icon_name) {
        w_gtk_button_set_icon_name (GTK_BUTTON(button), icon_name);
    }
    if (clicked_cb) {
        // void button_cb (GtkButton *button, gpointer user_data)
        g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (clicked_cb), cdata);
    }
    gtk_widget_set_can_default (button, TRUE);
    return button;
}


void w_gtk_widget_change_tooltip (GtkWidget *widget, const char *new_text)
{ /* changes widget tooltip only if the new_text is different */
    char *tip;
    tip = gtk_widget_get_tooltip_text (widget);
    if (!tip || g_strcmp0(tip,new_text) != 0) {
        gtk_widget_set_tooltip_text (widget, new_text);
    }
    g_free (tip);
}


/* ================================================== */
/*                  TREE VIEW                         */
/* ================================================== */

int w_gtk_tree_view_get_num_selected (GtkWidget *tv)
{
    GtkTreeView      *tree = GTK_TREE_VIEW (tv);
    GtkTreeSelection *tsel = gtk_tree_view_get_selection (tree);
    int              count = gtk_tree_selection_count_selected_rows (tsel);
    return count;
}


void w_gtk_tree_view_clear_list (GtkWidget *tv)
{
    GtkTreeView  *tree  = GTK_TREE_VIEW (tv);
    GtkListStore *store = GTK_LIST_STORE (gtk_tree_view_get_model (tree));
    gtk_list_store_clear (store);
}


void w_gtk_tree_view_clear_tree (GtkWidget *tv)
{
    GtkTreeView  *tree  = GTK_TREE_VIEW (tv);
    GtkTreeStore *store = GTK_TREE_STORE (gtk_tree_view_get_model (tree));
    gtk_tree_store_clear (store);
}


void w_gtk_tree_view_select_all (GtkWidget *tv)
{
    GtkTreeView      *tree = GTK_TREE_VIEW (tv);
    GtkTreeSelection *tsel = gtk_tree_view_get_selection (tree);
    gtk_tree_selection_select_all (tsel);
}


void w_gtk_tree_view_deselect_all (GtkWidget *tv)
{
    GtkTreeView      *tree = GTK_TREE_VIEW (tv);
    GtkTreeSelection *tsel = gtk_tree_view_get_selection (tree);
    gtk_tree_selection_unselect_all (tsel);
}


void w_gtk_tree_view_select_row (GtkWidget *tv, int n)
{
    GtkTreeView      *tree  = GTK_TREE_VIEW (tv);
    GtkTreeSelection *tsel  = gtk_tree_view_get_selection (tree);
    GtkTreePath      *tpath = gtk_tree_path_new_from_indices (n, -1);
    gtk_tree_selection_select_path (tsel, tpath);
    gtk_tree_path_free (tpath);
}

/* ================================================== */
/*                  COMBO BOX                         */
/* ================================================== */


void w_gtk_glist_to_combo (GtkComboBox *combo, GList *strings, int default_index)
{
    GList * list;
    char * text;
    int len = 0;
    gtk_combo_box_text_remove_all (GTK_COMBO_BOX_TEXT (combo));
    if (!strings) {
        return; // nothing to add
    }
    for (list = strings;  list;  list = list->next)
    {
        text = (char *) list->data;
        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), text);
        len++;
    }
    if (default_index >= len || default_index < 0) {
        default_index = 0;
    }
    gtk_combo_box_set_active (GTK_COMBO_BOX (combo), default_index);
}


void w_gtk_strv_to_combo (GtkComboBox *combo, char **strv, int default_index)
{
    int i;
    GtkListStore * store;
    GtkTreeIter iter;
    store = GTK_LIST_STORE (gtk_combo_box_get_model (combo));
    gtk_list_store_clear (store);
    //gtk_combo_box_text_remove_all (GTK_COMBO_BOX_TEXT (combo));
    if (!strv || !*strv) {
        return; /* nothing to add */
    }
    for (i = 0; strv[i]; i++) {
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter, 0, strv[i], -1);
        //gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), strv[i]);
    }
    if (default_index >= i || default_index < 0) {
        default_index = 0;
    }
    gtk_combo_box_set_active (GTK_COMBO_BOX (combo), default_index);
}


int w_gtk_combo_box_get_count (GtkComboBox *combo)
{
    GtkTreeModel * model;
    GtkTreeIter iter;
    gboolean valid;
    int count = 0;
    model = gtk_combo_box_get_model (combo);
    valid = gtk_tree_model_get_iter_first (model, &iter);
    while (valid) {
        count++;
        valid = gtk_tree_model_iter_next (model, &iter);
    }
    return count;
}


void w_gtk_combo_box_find_and_select (GtkComboBox *combo, char *str)
{
    GtkTreeModel * model;
    GtkTreeIter iter;
    gboolean valid;
    char *value;
    if (!str) {
        return;
    }
    model = gtk_combo_box_get_model (combo);
    valid = gtk_tree_model_get_iter_first (model, &iter);
    while (valid) {
        gtk_tree_model_get (model, &iter, 0, &value, -1);
        if (g_strcmp0 (value, str) == 0) {
            gtk_combo_box_set_active_iter (combo, &iter);
            break;
        }
        valid = gtk_tree_model_iter_next (model, &iter);
    }
}

/* ================================================== */
/*                   GTK < 3.0                        */
/* ================================================== */

#if ! GTK_CHECK_VERSION (3, 0, 0)

void gtk_widget_set_halign (GtkWidget *widget, GtkAlign align)
{
    // from gtk_misc_set_alignment ()
    GtkMisc * misc = GTK_MISC (widget);
    gfloat xalign = 0.0;
    if   (xalign == GTK_ALIGN_CENTER) xalign = 0.5;
    else if (xalign == GTK_ALIGN_END) xalign = 1.0;
    if (xalign != misc->xalign)
    {
        g_object_freeze_notify (G_OBJECT (misc));
        g_object_notify (G_OBJECT (misc), "xalign");
        misc->xalign = xalign;
        if (gtk_widget_is_drawable (widget))
            gtk_widget_queue_draw (widget);
        g_object_thaw_notify (G_OBJECT (misc));
    }
}

void gtk_widget_set_valign (GtkWidget *widget, GtkAlign align)
{
    // from gtk_misc_set_alignment ()
    GtkMisc * misc = GTK_MISC (widget);
    gfloat yalign = 0.0;
    if   (yalign == GTK_ALIGN_CENTER) yalign = 0.5;
    else if (yalign == GTK_ALIGN_END) yalign = 1.0;
    if (yalign != misc->yalign)
    {
        g_object_freeze_notify (G_OBJECT (misc));
        g_object_notify (G_OBJECT (misc), "yalign");
        misc->yalign = yalign;
        if (gtk_widget_is_drawable (widget))
            gtk_widget_queue_draw (widget);
        g_object_thaw_notify (G_OBJECT (misc));
    }
}

void gtk_widget_set_margin_start  (GtkWidget *widget, gint margin)
{
    // from gtk_misc_set_padding ()
    // deprecated since 3.12+: gtk_widget_set_margin_left
    // deprecated since 3.12+: gtk_widget_set_margin_right
    GtkRequisition *requisition;
    int xpad = margin;
    GtkMisc * misc = GTK_MISC (widget);
    if (xpad < 0) xpad = 0;
    if (xpad != misc->xpad)
    {
        g_object_freeze_notify (G_OBJECT (misc));
        if (xpad != misc->xpad)
            g_object_notify (G_OBJECT (misc), "xpad");
        requisition = &(GTK_WIDGET (misc)->requisition);
        requisition->width -= misc->xpad * 2;
        misc->xpad = xpad;
        requisition->width += misc->xpad * 2;
        if (gtk_widget_is_drawable (widget))
            gtk_widget_queue_resize (GTK_WIDGET (misc));
        g_object_thaw_notify (G_OBJECT (misc));
    }
}

void gtk_widget_set_margin_top (GtkWidget *widget, gint margin)
{
    // from gtk_misc_set_padding ()
    GtkRequisition *requisition;
    int ypad = margin;
    GtkMisc * misc = GTK_MISC (widget);
    if (ypad < 0) ypad = 0;
    if (ypad != misc->ypad)
    {
        g_object_freeze_notify (G_OBJECT (misc));
        if (ypad != misc->ypad)
            g_object_notify (G_OBJECT (misc), "ypad");
        requisition = &(GTK_WIDGET (misc)->requisition);
        requisition->height -= misc->ypad * 2;
        misc->ypad = ypad;
        requisition->height += misc->ypad * 2;
        if (gtk_widget_is_drawable (widget))
            gtk_widget_queue_resize (GTK_WIDGET (misc));
        g_object_thaw_notify (G_OBJECT (misc));
    }
}

#endif

