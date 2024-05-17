void DomOperationMessageSender::OnModalDialogShown() {
  if (automation_ && use_json_interface_) {
    AutomationJSONReply(automation_, reply_message_.release())
        .SendErrorCode(automation::kBlockedByModalDialog);
    delete this;
  }
}
