void RenderFrameHostImpl::OnRunBeforeUnloadConfirm(
    const GURL& frame_url,
    bool is_reload,
    IPC::Message* reply_msg) {
  GetProcess()->SetIgnoreInputEvents(true);

  for (RenderFrameHostImpl* frame = this; frame; frame = frame->GetParent()) {
    if (frame->beforeunload_timeout_)
      frame->beforeunload_timeout_->Stop();
  }

  delegate_->RunBeforeUnloadConfirm(this, is_reload, reply_msg);
}
