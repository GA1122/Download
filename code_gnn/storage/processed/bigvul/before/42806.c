static gboolean event_after(GtkWidget *text_view, GdkEvent *ev)
{
    GtkTextIter start, end, iter;
    GtkTextBuffer *buffer;
    GdkEventButton *event;
    gint x, y;

    if (ev->type != GDK_BUTTON_RELEASE)
        return FALSE;

    event = (GdkEventButton *)ev;

    if (event->button != GDK_BUTTON_PRIMARY)
        return FALSE;

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

     
    gtk_text_buffer_get_selection_bounds(buffer, &start, &end);
    if (gtk_text_iter_get_offset(&start) != gtk_text_iter_get_offset(&end))
        return FALSE;

    gtk_text_view_window_to_buffer_coords(GTK_TEXT_VIEW (text_view),
                                          GTK_TEXT_WINDOW_WIDGET,
                                          event->x, event->y, &x, &y);

    gtk_text_view_get_iter_at_location(GTK_TEXT_VIEW (text_view), &iter, x, y);

    open_browse_if_link(text_view, &iter);

    return FALSE;
}