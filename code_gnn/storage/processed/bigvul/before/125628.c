void RenderViewHostImpl::JavaScriptDialogClosed(IPC::Message* reply_msg,
                                                bool success,
                                                const string16& user_input) {
  GetProcess()->SetIgnoreInputEvents(false);
  bool is_waiting =
      is_waiting_for_beforeunload_ack_ || is_waiting_for_unload_ack_;

  if (is_waiting) {
    StartHangMonitorTimeout(TimeDelta::FromMilliseconds(
        success ? kUnloadTimeoutMS : hung_renderer_delay_ms_));
  }

  ViewHostMsg_RunJavaScriptMessage::WriteReplyParams(reply_msg,
                                                     success, user_input);
  Send(reply_msg);

  if (is_waiting && are_javascript_messages_suppressed_)
    delegate_->RendererUnresponsive(this, is_waiting);
}
