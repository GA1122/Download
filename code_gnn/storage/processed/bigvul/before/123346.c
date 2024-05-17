void RenderWidgetHostViewGtk::ShowCurrentCursor() {
  if (!gtk_widget_get_window(view_.get()))
    return;

  GdkCursor* gdk_cursor;
  if (current_cursor_.GetCursorType() == GDK_LAST_CURSOR) {
    gdk_cursor = is_loading_ ? GetMozSpinningCursor() : NULL;
  } else {
    gdk_cursor = current_cursor_.GetNativeCursor();
  }
  gdk_window_set_cursor(gtk_widget_get_window(view_.get()), gdk_cursor);
}
