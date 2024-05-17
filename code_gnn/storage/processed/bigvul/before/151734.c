bool Browser::ShouldCloseWindow() {
  if (!CanCloseWithInProgressDownloads())
    return false;

  if (IsFastTabUnloadEnabled())
    return fast_unload_controller_->ShouldCloseWindow();
  return unload_controller_->ShouldCloseWindow();
}
