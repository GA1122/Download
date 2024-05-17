void TestingAutomationProvider::ShowInterstitialPage(
    int tab_handle,
    const std::string& html_text,
    IPC::Message* reply_message) {
  if (tab_tracker_->ContainsHandle(tab_handle)) {
    NavigationController* controller = tab_tracker_->GetResource(tab_handle);
    WebContents* web_contents = controller->GetWebContents();

    new NavigationNotificationObserver(controller, this, reply_message, 1,
                                       false, false);

    AutomationInterstitialPage* interstitial =
        new AutomationInterstitialPage(web_contents,
                                       GURL("about:interstitial"),
                                       html_text);
    interstitial->Show();
    return;
  }

  AutomationMsg_ShowInterstitialPage::WriteReplyParams(
      reply_message, AUTOMATION_MSG_NAVIGATION_ERROR);
  Send(reply_message);
}
