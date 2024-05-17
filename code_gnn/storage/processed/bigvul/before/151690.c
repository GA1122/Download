void Browser::InProgressDownloadResponse(bool cancel_downloads) {
  if (cancel_downloads) {
    cancel_download_confirmation_state_ = RESPONSE_RECEIVED;
    chrome::CloseWindow(this);
    return;
  }

  cancel_download_confirmation_state_ = NOT_PROMPTED;

  chrome::ShowDownloads(this);

  if (IsFastTabUnloadEnabled())
    fast_unload_controller_->CancelWindowClose();
  else
    unload_controller_->CancelWindowClose();
}
