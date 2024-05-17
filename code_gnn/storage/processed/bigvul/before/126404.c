gboolean BrowserWindowGtk::OnMainWindowDeleteEvent(GtkWidget* widget,
                                                   GdkEvent* event) {
  Close();

  return TRUE;
}
