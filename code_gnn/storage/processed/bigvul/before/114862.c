void TestingAutomationProvider::CloseBrowser(int browser_handle,
                                             IPC::Message* reply_message) {
  if (!browser_tracker_->ContainsHandle(browser_handle))
    return;

  Browser* browser = browser_tracker_->GetResource(browser_handle);
  new BrowserClosedNotificationObserver(browser, this, reply_message);
  browser->window()->Close();
}
