void BrowserWindowGtk::Copy() {
  gtk_window_util::DoCopy(
      window_, chrome::GetActiveWebContents(browser_.get()));
}
