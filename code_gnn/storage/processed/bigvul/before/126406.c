gboolean BrowserWindowGtk::OnMouseMoveEvent(GtkWidget* widget,
                                            GdkEventMotion* event) {
  if (!UseCustomFrame() || event->window != gtk_widget_get_window(widget)) {
    if (frame_cursor_) {
      frame_cursor_ = NULL;
      gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(window_)), NULL);
    }
    return FALSE;
  }

  GdkWindowEdge edge;
  bool has_hit_edge = GetWindowEdge(static_cast<int>(event->x),
                                    static_cast<int>(event->y), &edge);
  GdkCursorType new_cursor = GDK_LAST_CURSOR;
  if (has_hit_edge)
    new_cursor = gtk_window_util::GdkWindowEdgeToGdkCursorType(edge);

  GdkCursorType last_cursor = GDK_LAST_CURSOR;
  if (frame_cursor_)
    last_cursor = frame_cursor_->type;

  if (last_cursor != new_cursor) {
    if (has_hit_edge) {
      frame_cursor_ = gfx::GetCursor(new_cursor);
    } else {
      frame_cursor_ = NULL;
    }
    gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(window_)),
                          frame_cursor_);
  }
  return FALSE;
}
