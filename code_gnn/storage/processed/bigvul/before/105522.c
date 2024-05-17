static gboolean webkit_web_view_show_help(GtkWidget* widget, GtkWidgetHelpType help_type)
{
    if (help_type == GTK_WIDGET_HELP_TOOLTIP)
        gtk_widget_set_has_tooltip(widget, TRUE);

    return GTK_WIDGET_CLASS(webkit_web_view_parent_class)->show_help(widget, help_type);
}
