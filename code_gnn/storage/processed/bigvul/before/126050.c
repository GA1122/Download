void PageSnapshotTaker::SendMessage(bool success,
                                    const std::string& error_msg) {
  if (automation_) {
    if (success) {
      AutomationJSONReply(automation_, reply_message_.release())
          .SendSuccess(NULL);
    } else {
      AutomationJSONReply(automation_, reply_message_.release())
          .SendError("Failed to take snapshot of page: " + error_msg);
    }
  }
  delete this;
}
