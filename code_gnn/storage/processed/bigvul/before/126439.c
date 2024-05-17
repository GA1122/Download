void BrowserWindowGtk::ShowSupportedWindowFeatures() {
  if (IsTabStripSupported())
    tabstrip_->Show();

  if (IsToolbarSupported()) {
    toolbar_->Show();
    gtk_widget_show(toolbar_border_);
    gdk_window_lower(gtk_widget_get_window(toolbar_border_));
  }

  if (IsBookmarkBarSupported())
    MaybeShowBookmarkBar(false);
}
