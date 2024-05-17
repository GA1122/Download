void Browser::InProgressDownloadResponse(bool cancel_downloads) {
  if (cancel_downloads) {
    cancel_download_confirmation_state_ = RESPONSE_RECEIVED;
    CloseWindow();
    return;
  }

  cancel_download_confirmation_state_ = NOT_PROMPTED;

  ShowDownloadsTab();
}
