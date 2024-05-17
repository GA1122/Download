void RenderFrameHostImpl::PendingDeletionCheckCompleted() {
  if (unload_state_ == UnloadState::Completed && children_.empty()) {
    if (is_waiting_for_swapout_ack_)
      OnSwappedOut();
    else
      parent_->RemoveChild(frame_tree_node_);
  }
}
