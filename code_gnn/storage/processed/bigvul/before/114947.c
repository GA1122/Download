void TestingAutomationProvider::GetShelfVisibility(int handle, bool* visible) {
  *visible = false;

  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    if (browser) {
#if defined(OS_CHROMEOS)
      *visible = ActiveDownloadsUI::GetPopup();
#else
      *visible = browser->window()->IsDownloadShelfVisible();
#endif
    }
  }
}
