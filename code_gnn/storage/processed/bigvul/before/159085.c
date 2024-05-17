void ChromeDownloadManagerDelegate::Shutdown() {
  download_prefs_.reset();
  weak_ptr_factory_.InvalidateWeakPtrs();
  download_manager_ = NULL;
}
