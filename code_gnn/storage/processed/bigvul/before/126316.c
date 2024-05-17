void BrowserWindowGtk::ConfirmBrowserCloseWithPendingDownloads() {
  DownloadInProgressDialogGtk::Show(browser(), GetNativeWindow());
}
