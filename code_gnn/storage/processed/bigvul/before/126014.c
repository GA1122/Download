void DomOperationMessageSender::OnDomOperationCompleted(
    const std::string& json) {
  if (automation_) {
    if (use_json_interface_) {
      DictionaryValue dict;
      dict.SetString("result", json);
      AutomationJSONReply(automation_, reply_message_.release())
          .SendSuccess(&dict);
    } else {
      AutomationMsg_DomOperation::WriteReplyParams(reply_message_.get(), json);
      automation_->Send(reply_message_.release());
    }
  }
  delete this;
}
