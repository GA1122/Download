void RenderFrameHostImpl::OnSwappedOut() {
  if (!is_waiting_for_swapout_ack_)
    return;

  TRACE_EVENT_ASYNC_END0("navigation", "RenderFrameHostImpl::SwapOut", this);
  if (swapout_event_monitor_timeout_)
    swapout_event_monitor_timeout_->Stop();

  ClearAllWebUI();

  if (frame_tree_node_->IsMainFrame() && !render_view_host_->is_active())
    render_view_host_->set_is_swapped_out(true);

  bool deleted =
      frame_tree_node_->render_manager()->DeleteFromPendingList(this);
  CHECK(deleted);
}
