bool Browser::IsAttemptingToCloseBrowser() const {
  return unload_controller_->is_attempting_to_close_browser();
}
