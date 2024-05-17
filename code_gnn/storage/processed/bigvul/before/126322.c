void BrowserWindowGtk::Cut() {
  gtk_window_util::DoCut(
      window_, chrome::GetActiveWebContents(browser_.get()));
}
