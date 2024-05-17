void RenderViewHostImpl::FirePageBeforeUnload(bool for_cross_site_transition) {
  if (!IsRenderViewLive()) {
    is_waiting_for_beforeunload_ack_ = true;   
    unload_ack_is_for_cross_site_transition_ = for_cross_site_transition;
    base::TimeTicks now = base::TimeTicks::Now();
    OnShouldCloseACK(true, now, now);
    return;
  }

  if (is_waiting_for_beforeunload_ack_) {
    unload_ack_is_for_cross_site_transition_ =
        unload_ack_is_for_cross_site_transition_ && for_cross_site_transition;
  } else {
    is_waiting_for_beforeunload_ack_ = true;
    unload_ack_is_for_cross_site_transition_ = for_cross_site_transition;
    increment_in_flight_event_count();
    StartHangMonitorTimeout(TimeDelta::FromMilliseconds(kUnloadTimeoutMS));
    send_should_close_start_time_ = base::TimeTicks::Now();
    Send(new ViewMsg_ShouldClose(GetRoutingID()));
  }
}
