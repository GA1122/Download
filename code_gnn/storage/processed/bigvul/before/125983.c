void BrowserClosedNotificationObserver::Observe(
    int type, const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK_EQ(chrome::NOTIFICATION_BROWSER_CLOSED, type);

  if (!automation_) {
    delete this;
    return;
  }

  int browser_count = static_cast<int>(BrowserList::size());
  bool app_closing = browser_count == 1;

  if (use_json_interface_) {
    AutomationJSONReply(automation_,
                        reply_message_.release()).SendSuccess(NULL);
  } else {
    if (for_browser_command_) {
      AutomationMsg_WindowExecuteCommand::WriteReplyParams(reply_message_.get(),
                                                           true);
    } else {
      AutomationMsg_CloseBrowser::WriteReplyParams(reply_message_.get(), true,
                                                   app_closing);
    }
    automation_->Send(reply_message_.release());
  }
  delete this;
}
