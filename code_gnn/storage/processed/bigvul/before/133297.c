void HandleTakePartialScreenshot(ScreenshotDelegate* screenshot_delegate) {
  base::RecordAction(UserMetricsAction("Accel_Take_Partial_Screenshot"));
  DCHECK(screenshot_delegate);
  Shell::GetInstance()->screenshot_controller()->StartPartialScreenshotSession(
      screenshot_delegate, true  );
}
