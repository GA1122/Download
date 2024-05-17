void NavigationControllerImpl::SetScreenshotManager(
    std::unique_ptr<NavigationEntryScreenshotManager> manager) {
  if (manager.get())
    screenshot_manager_ = std::move(manager);
  else
    screenshot_manager_.reset(new NavigationEntryScreenshotManager(this));
}
