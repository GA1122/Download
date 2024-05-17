void BrowserWindowGtk::Paste() {
  gtk_window_util::DoPaste(
      window_, chrome::GetActiveWebContents(browser_.get()));
}
