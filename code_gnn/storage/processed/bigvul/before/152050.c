void RenderFrameHostImpl::OnSwapOutACK() {
  if (frame_tree_node_->render_manager()->is_attaching_inner_delegate()) {
    SetRenderFrameCreated(false);
    return;
  }

  if (!is_waiting_for_swapout_ack_)
    return;

  DCHECK_EQ(UnloadState::InProgress, unload_state_);
  unload_state_ = UnloadState::Completed;
  PendingDeletionCheckCompleted();   
}
