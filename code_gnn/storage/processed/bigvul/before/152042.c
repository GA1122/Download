void RenderFrameHostImpl::OnRunBeforeUnloadConfirm(
    bool is_reload,
    IPC::Message* reply_msg) {
  TRACE_EVENT1("navigation", "RenderFrameHostImpl::OnRunBeforeUnloadConfirm",
               "frame_tree_node", frame_tree_node_->frame_tree_node_id());

  RenderFrameHostImpl* beforeunload_initiator = GetBeforeUnloadInitiator();
  if (beforeunload_initiator) {
    if (beforeunload_initiator->beforeunload_dialog_request_cancels_unload_) {
      SendJavaScriptDialogReply(reply_msg, false  ,
                                base::string16());
      return;
    }

    if (beforeunload_initiator->has_shown_beforeunload_dialog_) {
      SendJavaScriptDialogReply(reply_msg, true  ,
                                base::string16());
      return;
    }
    beforeunload_initiator->has_shown_beforeunload_dialog_ = true;
  } else {
  }

  GetProcess()->SetBlocked(true);

  for (RenderFrameHostImpl* frame = this; frame; frame = frame->GetParent()) {
    if (frame->beforeunload_timeout_)
      frame->beforeunload_timeout_->Stop();
  }

  delegate_->RunBeforeUnloadConfirm(this, is_reload, reply_msg);
}
