void TestingAutomationProvider::ActionOnSSLBlockingPage(
    int handle,
    bool proceed,
    IPC::Message* reply_message) {
  if (tab_tracker_->ContainsHandle(handle)) {
    NavigationController* tab = tab_tracker_->GetResource(handle);
    NavigationEntry* entry = tab->GetActiveEntry();
    if (entry->GetPageType() == content::PAGE_TYPE_INTERSTITIAL) {
      WebContents* web_contents = tab->GetWebContents();
      InterstitialPage* ssl_blocking_page =
          InterstitialPage::GetInterstitialPage(web_contents);
      if (ssl_blocking_page) {
        if (proceed) {
          new NavigationNotificationObserver(tab, this, reply_message, 1,
                                             false, false);
          ssl_blocking_page->Proceed();
          return;
        }
        ssl_blocking_page->DontProceed();
        AutomationMsg_ActionOnSSLBlockingPage::WriteReplyParams(
            reply_message, AUTOMATION_MSG_NAVIGATION_SUCCESS);
        Send(reply_message);
        return;
      }
    }
  }
  AutomationMsg_ActionOnSSLBlockingPage::WriteReplyParams(
      reply_message, AUTOMATION_MSG_NAVIGATION_ERROR);
  Send(reply_message);
}
