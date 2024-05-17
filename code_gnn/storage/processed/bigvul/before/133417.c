void PaletteDelegateChromeOS::TakePartialScreenshot() {
  auto* screenshot_controller =
      ash::Shell::GetInstance()->screenshot_controller();
  auto* screenshot_delegate = ash::Shell::GetInstance()
                                  ->accelerator_controller_delegate()
                                  ->screenshot_delegate();

  screenshot_controller->set_pen_events_only(true);
  screenshot_controller->StartPartialScreenshotSession(
      screenshot_delegate, false  );
}
