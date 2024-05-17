void TabClosedNotificationObserver::ObserveTab(
    NavigationController* controller) {
  if (!automation_)
    return;

  if (use_json_interface_) {
    AutomationJSONReply(automation_,
                        reply_message_.release()).SendSuccess(NULL);
  } else {
    if (for_browser_command_) {
      AutomationMsg_WindowExecuteCommand::WriteReplyParams(reply_message_.get(),
                                                           true);
    } else {
      AutomationMsg_CloseTab::WriteReplyParams(reply_message_.get(), true);
    }
    automation_->Send(reply_message_.release());
  }
}
