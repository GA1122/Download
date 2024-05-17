void RenderFrameHostImpl::OnDetach() {
  if (!parent_) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RFH_DETACH_MAIN_FRAME);
    return;
  }

  if (!is_active() && !is_waiting_for_swapout_ack_) {
    unload_state_ = UnloadState::Completed;
    PendingDeletionCheckCompleted();
    return;
  }

  parent_->RemoveChild(frame_tree_node_);
}
