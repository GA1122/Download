void BrowserWindowGtk::ResetCustomFrameCursor() {
  if (!frame_cursor_)
    return;

  frame_cursor_ = NULL;
  gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(window_)), NULL);
}
