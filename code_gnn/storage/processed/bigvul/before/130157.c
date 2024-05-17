void RenderFrameHostImpl::DispatchBeforeUnload(bool for_cross_site_transition) {
  DCHECK(!GetParent());

  if (!render_view_host_->IsRenderViewLive()) {
    render_view_host_->is_waiting_for_beforeunload_ack_ = true;
    render_view_host_->unload_ack_is_for_cross_site_transition_ =
        for_cross_site_transition;
    base::TimeTicks now = base::TimeTicks::Now();
    OnBeforeUnloadACK(true, now, now);
    return;
  }

  if (render_view_host_->is_waiting_for_beforeunload_ack_) {
    render_view_host_->unload_ack_is_for_cross_site_transition_ =
        render_view_host_->unload_ack_is_for_cross_site_transition_ &&
        for_cross_site_transition;
  } else {
    render_view_host_->is_waiting_for_beforeunload_ack_ = true;
    render_view_host_->unload_ack_is_for_cross_site_transition_ =
        for_cross_site_transition;
    render_view_host_->increment_in_flight_event_count();
    render_view_host_->StartHangMonitorTimeout(
        TimeDelta::FromMilliseconds(RenderViewHostImpl::kUnloadTimeoutMS));
    send_before_unload_start_time_ = base::TimeTicks::Now();
    Send(new FrameMsg_BeforeUnload(routing_id_));
  }
}
