void RenderViewHostImpl::OnRunBeforeUnloadConfirm(const GURL& frame_url,
                                                  const string16& message,
                                                  bool is_reload,
                                                  IPC::Message* reply_msg) {
  GetProcess()->SetIgnoreInputEvents(true);
  StopHangMonitorTimeout();
  delegate_->RunBeforeUnloadConfirm(this, message, is_reload, reply_msg);
}
