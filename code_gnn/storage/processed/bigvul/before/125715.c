void RenderViewHostImpl::Shutdown() {
  if (run_modal_reply_msg_) {
    Send(run_modal_reply_msg_);
    run_modal_reply_msg_ = NULL;
    RenderViewHostImpl* opener =
        RenderViewHostImpl::FromID(GetProcess()->GetID(), run_modal_opener_id_);
    if (opener) {
      opener->StartHangMonitorTimeout(TimeDelta::FromMilliseconds(
          hung_renderer_delay_ms_));
      opener->increment_in_flight_event_count();
    }
    run_modal_opener_id_ = MSG_ROUTING_NONE;
  }

  RenderWidgetHostImpl::Shutdown();
}
