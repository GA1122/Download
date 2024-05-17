void Browser::ResetTryToCloseWindow() {
  cancel_download_confirmation_state_ = NOT_PROMPTED;
  if (IsFastTabUnloadEnabled())
    fast_unload_controller_->ResetTryToCloseWindow();
  else
    unload_controller_->ResetTryToCloseWindow();
}
