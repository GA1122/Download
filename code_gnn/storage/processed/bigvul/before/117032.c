void OmniboxPopupViewGtk::PaintUpdatesNow() {
  GdkWindow* gdk_window = gtk_widget_get_window(window_);
  gdk_window_process_updates(gdk_window, FALSE);
}
