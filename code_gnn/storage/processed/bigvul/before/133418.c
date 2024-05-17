 void PaletteDelegateChromeOS::TakeScreenshot() {
   auto* screenshot_delegate = ash::Shell::GetInstance()
                                   ->accelerator_controller_delegate()
                                  ->screenshot_delegate();
  screenshot_delegate->HandleTakeScreenshotForAllRootWindows();
}
