void RenderViewHostImpl::OnRunJavaScriptMessage(
    const string16& message,
    const string16& default_prompt,
    const GURL& frame_url,
    JavaScriptMessageType type,
    IPC::Message* reply_msg) {
  GetProcess()->SetIgnoreInputEvents(true);
  StopHangMonitorTimeout();
  delegate_->RunJavaScriptMessage(this, message, default_prompt, frame_url,
                                  type, reply_msg,
                                  &are_javascript_messages_suppressed_);
}
