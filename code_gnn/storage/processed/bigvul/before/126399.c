gboolean BrowserWindowGtk::OnFocusIn(GtkWidget* widget,
                                     GdkEventFocus* event) {
  BrowserList::SetLastActive(browser_.get());
  return FALSE;
}
