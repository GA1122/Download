void TestingAutomationProvider::GetActiveWindow(int* handle) {
  *handle = 0;
  Browser* browser = BrowserList::GetLastActive();
  if (browser) {
    gfx::NativeWindow window = browser->window()->GetNativeHandle();
    *handle = window_tracker_->Add(window);
  }
}
