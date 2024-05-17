void ChromeDownloadManagerDelegate::Shutdown() {
  download_history_.reset();
  download_prefs_.reset();
}
