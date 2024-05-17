void NavigationControllerImpl::SetScreenshotManager(
    WebContentsScreenshotManager* manager) {
  screenshot_manager_.reset(manager ? manager :
                            new WebContentsScreenshotManager(this));
}
