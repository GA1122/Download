void Browser::ResetBeforeUnloadHandlers() {
  cancel_download_confirmation_state_ = NOT_PROMPTED;
  if (IsFastTabUnloadEnabled())
    fast_unload_controller_->ResetBeforeUnloadHandlers();
  else
    unload_controller_->ResetBeforeUnloadHandlers();
}
