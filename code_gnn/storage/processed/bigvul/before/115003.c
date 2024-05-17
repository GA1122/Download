void TestingAutomationProvider::PrintNow(int tab_handle,
                                         IPC::Message* reply_message) {
  NavigationController* tab = NULL;
  WebContents* web_contents = GetWebContentsForHandle(tab_handle, &tab);
  if (web_contents) {
    FindAndActivateTab(tab);

    content::NotificationObserver* observer =
        new DocumentPrintedNotificationObserver(this, reply_message);

    TabContentsWrapper* wrapper =
        TabContentsWrapper::GetCurrentWrapperForContents(web_contents);
    if (!wrapper->print_view_manager()->PrintNow()) {
      delete observer;
    }

    return;
  }

  AutomationMsg_PrintNow::WriteReplyParams(reply_message, false);
  Send(reply_message);
}
