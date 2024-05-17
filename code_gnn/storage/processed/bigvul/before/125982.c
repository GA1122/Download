void BrowserOpenedNotificationObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (!automation_) {
    delete this;
    return;
  }

  if (type == chrome::NOTIFICATION_BROWSER_OPENED) {
    new_window_id_ = ExtensionTabUtil::GetWindowId(
        content::Source<Browser>(source).ptr());
  } else {
    DCHECK_EQ(content::NOTIFICATION_LOAD_STOP, type);
    NavigationController* controller =
        content::Source<NavigationController>(source).ptr();
    SessionTabHelper* session_tab_helper =
        SessionTabHelper::FromWebContents(controller->GetWebContents());
    int window_id = session_tab_helper ? session_tab_helper->window_id().id()
                                       : -1;
    if (window_id == new_window_id_) {
      if (use_json_interface_) {
        AutomationJSONReply(automation_,
                            reply_message_.release()).SendSuccess(NULL);
      } else {
        if (for_browser_command_) {
          AutomationMsg_WindowExecuteCommand::WriteReplyParams(
              reply_message_.get(), true);
        }
        automation_->Send(reply_message_.release());
      }
      delete this;
      return;
    }
  }
}
