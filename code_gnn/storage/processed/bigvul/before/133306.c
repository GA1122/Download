void AcceleratorControllerDelegateAura::SetScreenshotDelegate(
    std::unique_ptr<ScreenshotDelegate> screenshot_delegate) {
  screenshot_delegate_ = std::move(screenshot_delegate);
}
