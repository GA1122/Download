void TestingAutomationProvider::NavigateToURLBlockUntilNavigationsComplete(
    int handle, const GURL& url, int number_of_navigations,
    IPC::Message* reply_message) {
  if (tab_tracker_->ContainsHandle(handle)) {
    NavigationController* tab = tab_tracker_->GetResource(handle);

    Browser* browser = FindAndActivateTab(tab);

    if (browser) {
      new NavigationNotificationObserver(tab, this, reply_message,
                                         number_of_navigations, false, false);

      OpenURLParams params(
          url, Referrer(), CURRENT_TAB, content::PAGE_TRANSITION_TYPED, false);
      browser->OpenURL(params);
      return;
    }
  }

  AutomationMsg_NavigateToURLBlockUntilNavigationsComplete::WriteReplyParams(
      reply_message, AUTOMATION_MSG_NAVIGATION_ERROR);
  Send(reply_message);
}
