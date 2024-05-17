void BrowserWindowGtk::ExitFullscreen() {
  bool unmaximize_before_unfullscreen = IsMaximized() &&
      ui::GuessWindowManager() == ui::WM_METACITY;
  if (unmaximize_before_unfullscreen)
    UnMaximize();

  gtk_window_unfullscreen(window_);

  if (unmaximize_before_unfullscreen)
    gtk_window_maximize(window_);
}
