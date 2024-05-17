void RenderViewHostImpl::OnShouldCloseACK(
    bool proceed,
    const base::TimeTicks& renderer_before_unload_start_time,
    const base::TimeTicks& renderer_before_unload_end_time) {
  decrement_in_flight_event_count();
  StopHangMonitorTimeout();
  if (!is_waiting_for_beforeunload_ack_ || is_swapped_out_)
    return;

  is_waiting_for_beforeunload_ack_ = false;

  RenderViewHostDelegate::RendererManagement* management_delegate =
      delegate_->GetRendererManagementDelegate();
  if (management_delegate) {
    base::TimeTicks before_unload_end_time;
    if (!send_should_close_start_time_.is_null() &&
        !renderer_before_unload_start_time.is_null() &&
        !renderer_before_unload_end_time.is_null()) {
      InterProcessTimeTicksConverter converter(
          LocalTimeTicks::FromTimeTicks(send_should_close_start_time_),
          LocalTimeTicks::FromTimeTicks(base::TimeTicks::Now()),
          RemoteTimeTicks::FromTimeTicks(renderer_before_unload_start_time),
          RemoteTimeTicks::FromTimeTicks(renderer_before_unload_end_time));
      LocalTimeTicks browser_before_unload_end_time =
          converter.ToLocalTimeTicks(
              RemoteTimeTicks::FromTimeTicks(renderer_before_unload_end_time));
      before_unload_end_time = browser_before_unload_end_time.ToTimeTicks();
    }
    management_delegate->ShouldClosePage(
        unload_ack_is_for_cross_site_transition_, proceed,
        before_unload_end_time);
  }

  if (!proceed)
    delegate_->DidCancelLoading();
}
