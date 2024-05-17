void BrowserView::ConfirmBrowserCloseWithPendingDownloads(
    int download_count,
    Browser::DownloadClosePreventionType dialog_type,
    bool app_modal,
    const base::Callback<void(bool)>& callback) {
  DownloadInProgressDialogView::Show(
      GetNativeWindow(), download_count, dialog_type, app_modal, callback);
}
