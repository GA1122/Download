void BrowserWindowGtk::InvalidateWindow() {
  GtkAllocation allocation;
  gtk_widget_get_allocation(GTK_WIDGET(window_), &allocation);
  gdk_window_invalidate_rect(gtk_widget_get_window(GTK_WIDGET(window_)),
                             &allocation, TRUE);
}
