static void wrap_fixer(GtkWidget *widget, gpointer data_unused)
{
    if (GTK_IS_CONTAINER(widget))
    {
        gtk_container_foreach((GtkContainer*)widget, wrap_fixer, NULL);
        return;
    }
    if (GTK_IS_LABEL(widget))
    {
        GtkLabel *label = (GtkLabel*)widget;
#if ((GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION < 13) || (GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION == 13 && GTK_MICRO_VERSION < 5))
        GtkMisc *misc = (GtkMisc*)widget;
        gfloat yalign;  
        gint ypad;  
        if (gtk_label_get_line_wrap(label)
         && (gtk_misc_get_alignment(misc, NULL, &yalign), yalign == 0)
         && (gtk_misc_get_padding(misc, NULL, &ypad), ypad == 0)
#else
        if (gtk_label_get_line_wrap(label)
         && (gtk_widget_get_halign(widget) == GTK_ALIGN_START)
         && (gtk_widget_get_margin_top(widget) == 0)
         && (gtk_widget_get_margin_bottom(widget) == 0)
#endif
        ) {
            make_label_autowrap_on_resize(label);
            return;
        }
    }
}
