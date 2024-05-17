bool Browser::IsAttemptingToCloseBrowser() const {
  if (IsFastTabUnloadEnabled())
    return fast_unload_controller_->is_attempting_to_close_browser();
  return unload_controller_->is_attempting_to_close_browser();
}
