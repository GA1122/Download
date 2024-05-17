void TestingAutomationProvider::OpenNewBrowserWindowOfType(
    int type, bool show, IPC::Message* reply_message) {
  new BrowserOpenedNotificationObserver(this, reply_message);
  Browser* browser = new Browser(static_cast<Browser::Type>(type), profile_);
  browser->InitBrowserWindow();
  browser->AddBlankTab(true);
  if (show)
    browser->window()->Show();
}
