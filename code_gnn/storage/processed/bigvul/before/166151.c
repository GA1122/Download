void RenderFrameHostImpl::OnSwapOutACK() {
  if (!is_waiting_for_swapout_ack_)
    return;

  DCHECK_EQ(UnloadState::InProgress, unload_state_);
  unload_state_ = UnloadState::Completed;
  PendingDeletionCheckCompleted();
}
