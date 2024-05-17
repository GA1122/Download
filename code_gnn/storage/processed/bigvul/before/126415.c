void BrowserWindowGtk::Restore() {
  if (IsMaximized())
    UnMaximize();
  else if (IsMinimized())
    gtk_window_deiconify(window_);
}
