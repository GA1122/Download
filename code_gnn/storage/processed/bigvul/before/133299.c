void HandleTakeWindowScreenshot(ScreenshotDelegate* screenshot_delegate) {
  base::RecordAction(UserMetricsAction("Accel_Take_Window_Screenshot"));
  DCHECK(screenshot_delegate);
  Shell::GetInstance()->screenshot_controller()->StartWindowScreenshotSession(
      screenshot_delegate);
}
