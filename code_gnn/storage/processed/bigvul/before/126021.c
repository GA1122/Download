void DomOperationMessageSender::OnJavascriptBlocked() {
  if (automation_ && use_json_interface_) {
    AutomationJSONReply(automation_, reply_message_.release())
        .SendError("Javascript execution was blocked");
    delete this;
  }
}
