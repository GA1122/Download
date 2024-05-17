void TestingAutomationProvider::SetShelfVisibility(int handle, bool visible) {
  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    if (browser) {
#if defined(OS_CHROMEOS)
      Browser* popup_browser = ActiveDownloadsUI::GetPopup();
      if (!popup_browser && visible)
        ActiveDownloadsUI::OpenPopup(browser->profile());
      if (popup_browser && !visible)
        popup_browser->CloseWindow();
#else
      if (visible)
        browser->window()->GetDownloadShelf()->Show();
      else
        browser->window()->GetDownloadShelf()->Close();
#endif
    }
  }
}
