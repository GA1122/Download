void AutomationProviderBookmarkModelObserver::ReplyAndDelete(bool success) {
  if (automation_provider_) {
    if (use_json_interface_) {
      AutomationJSONReply(automation_provider_,
                          reply_message_.release()).SendSuccess(NULL);
    } else {
      AutomationMsg_WaitForBookmarkModelToLoad::WriteReplyParams(
          reply_message_.get(), success);
      automation_provider_->Send(reply_message_.release());
    }
  }
  delete this;
}
