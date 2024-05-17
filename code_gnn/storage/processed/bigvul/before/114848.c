void TestingAutomationProvider::AppendBackgroundTab(
    int handle,
    const GURL& url,
    IPC::Message* reply_message) {
  int append_tab_response = -1;   
  content::NotificationObserver* observer = NULL;

  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    observer = new TabAppendedNotificationObserver(browser, this,
                                                   reply_message);

    browser::NavigateParams params(browser, url, content::PAGE_TRANSITION_LINK);
    params.disposition = NEW_BACKGROUND_TAB;
    browser::Navigate(&params);
    TabContentsWrapper* contents = params.target_contents;
    if (contents) {
      append_tab_response = GetIndexForNavigationController(
          &contents->web_contents()->GetController(), browser);
    }
  }

  if (append_tab_response < 0) {
    if (observer)
      delete observer;

    AutomationMsg_AppendBackgroundTab::WriteReplyParams(reply_message,
                                                        append_tab_response);
    Send(reply_message);
  }
}
