void RenderFrameHostImpl::DeleteRenderFrame() {
  if (!is_active())
    return;

  if (render_frame_created_) {
    Send(new FrameMsg_Delete(routing_id_));

    if (!frame_tree_node_->IsMainFrame() && IsCurrent() &&
        GetSuddenTerminationDisablerState(blink::kUnloadHandler)) {
      RenderProcessHostImpl* process =
          static_cast<RenderProcessHostImpl*>(GetProcess());
      process->DelayProcessShutdownForUnload(subframe_unload_timeout_);
    }
  }

  unload_state_ = GetSuddenTerminationDisablerState(blink::kUnloadHandler)
                      ? UnloadState::InProgress
                      : UnloadState::Completed;
}
