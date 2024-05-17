void ExecuteBrowserCommandObserver::Observe(
    int type, const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (type == notification_type_) {
    if (automation_) {
      if (use_json_interface_) {
        AutomationJSONReply(automation_,
                            reply_message_.release()).SendSuccess(NULL);
      } else {
        AutomationMsg_WindowExecuteCommand::WriteReplyParams(
            reply_message_.get(), true);
        automation_->Send(reply_message_.release());
      }
    }
    delete this;
  } else {
    NOTREACHED();
  }
}
