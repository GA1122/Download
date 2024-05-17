static void add_warning(const char *warning)
{
    char *label_str = xasprintf("• %s", warning);
    GtkWidget *warning_lbl = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(warning_lbl), label_str);
     
    free(label_str);

#if ((GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION < 13) || (GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION == 13 && GTK_MICRO_VERSION < 5))
    gtk_misc_set_alignment(GTK_MISC(warning_lbl), 0.0, 0.0);
#else
    gtk_widget_set_halign (warning_lbl, GTK_ALIGN_START);
    gtk_widget_set_valign (warning_lbl, GTK_ALIGN_END);
#endif
    gtk_label_set_justify(GTK_LABEL(warning_lbl), GTK_JUSTIFY_LEFT);
    gtk_label_set_line_wrap(GTK_LABEL(warning_lbl), TRUE);

    add_widget_to_warning_area(warning_lbl);
}