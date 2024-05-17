void RenderFrameHostImpl::OnRunJavaScriptDialog(
    const base::string16& message,
    const base::string16& default_prompt,
    JavaScriptDialogType dialog_type,
    IPC::Message* reply_msg) {
  if (IsWaitingForUnloadACK()) {
    SendJavaScriptDialogReply(reply_msg, true, base::string16());
    return;
  }

  GetProcess()->SetBlocked(true);

  delegate_->RunJavaScriptDialog(this, message, default_prompt, dialog_type,
                                 reply_msg);
}
