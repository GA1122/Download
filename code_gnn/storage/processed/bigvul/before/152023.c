void RenderFrameHostImpl::OnDetach() {
  if (!parent_) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RFH_DETACH_MAIN_FRAME);
    return;
  }

  if (is_waiting_for_swapout_ack_) {
    parent_->RemoveChild(frame_tree_node_);
    return;
  }

  if (unload_state_ != UnloadState::NotRun) {
    unload_state_ = UnloadState::Completed;
    PendingDeletionCheckCompleted();   
    return;
  }

  unload_state_ = UnloadState::Completed;
  StartPendingDeletionOnSubtree();
  PendingDeletionCheckCompletedOnSubtree();   
}
