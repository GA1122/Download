void TestingAutomationProvider::AppendTab(int handle,
                                          const GURL& url,
                                          IPC::Message* reply_message) {
  int append_tab_response = -1;   
  content::NotificationObserver* observer = NULL;

  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    observer = new TabAppendedNotificationObserver(browser, this,
                                                   reply_message);
    TabContentsWrapper* contents =
        browser->AddSelectedTabWithURL(url, content::PAGE_TRANSITION_TYPED);
    if (contents) {
      append_tab_response = GetIndexForNavigationController(
          &contents->web_contents()->GetController(), browser);
    }
  }

  if (append_tab_response < 0) {

    if (observer)
      delete observer;

    AutomationMsg_AppendTab::WriteReplyParams(reply_message,
                                              append_tab_response);
    Send(reply_message);
  }
}
