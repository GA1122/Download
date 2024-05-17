void RenderFrameHostImpl::OnBeforeUnloadACK(
    bool proceed,
    const base::TimeTicks& renderer_before_unload_start_time,
    const base::TimeTicks& renderer_before_unload_end_time) {
  if (GetParent()) {
    NOTREACHED() << "Should only receive BeforeUnload_ACK from the main frame.";
    return;
  }

  render_view_host_->decrement_in_flight_event_count();
  render_view_host_->StopHangMonitorTimeout();
  if (!render_view_host_->is_waiting_for_beforeunload_ack_ ||
      render_view_host_->rvh_state_ != RenderViewHostImpl::STATE_DEFAULT) {
    return;
  }

  render_view_host_->is_waiting_for_beforeunload_ack_ = false;

  base::TimeTicks before_unload_end_time;
  if (!send_before_unload_start_time_.is_null() &&
      !renderer_before_unload_start_time.is_null() &&
      !renderer_before_unload_end_time.is_null()) {
    InterProcessTimeTicksConverter converter(
        LocalTimeTicks::FromTimeTicks(send_before_unload_start_time_),
        LocalTimeTicks::FromTimeTicks(base::TimeTicks::Now()),
        RemoteTimeTicks::FromTimeTicks(renderer_before_unload_start_time),
        RemoteTimeTicks::FromTimeTicks(renderer_before_unload_end_time));
    LocalTimeTicks browser_before_unload_end_time =
        converter.ToLocalTimeTicks(
            RemoteTimeTicks::FromTimeTicks(renderer_before_unload_end_time));
    before_unload_end_time = browser_before_unload_end_time.ToTimeTicks();
  }
  frame_tree_node_->render_manager()->OnBeforeUnloadACK(
      render_view_host_->unload_ack_is_for_cross_site_transition_, proceed,
      before_unload_end_time);

  if (!proceed)
    render_view_host_->GetDelegate()->DidCancelLoading();
}
