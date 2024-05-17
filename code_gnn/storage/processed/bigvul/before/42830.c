static void label_wrapper(GtkWidget *widget, gpointer data_unused)
{
    if (GTK_IS_CONTAINER(widget))
    {
        gtk_container_foreach((GtkContainer*)widget, label_wrapper, NULL);
        return;
    }
    if (GTK_IS_LABEL(widget))
    {
        GtkLabel *label = (GtkLabel*)widget;
        gtk_label_set_line_wrap(label, 1);
    }
}