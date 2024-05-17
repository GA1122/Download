void TestingAutomationProvider::GetBrowserForWindow(int window_handle,
                                                    bool* success,
                                                    int* browser_handle) {
  *success = false;
  *browser_handle = 0;

  gfx::NativeWindow window = window_tracker_->GetResource(window_handle);
  if (!window)
    return;

  BrowserList::const_iterator iter = BrowserList::begin();
  for (;iter != BrowserList::end(); ++iter) {
    gfx::NativeWindow this_window = (*iter)->window()->GetNativeHandle();
    if (window == this_window) {
      *browser_handle = browser_tracker_->Add(*iter);
      *success = true;
      return;
    }
  }
}
