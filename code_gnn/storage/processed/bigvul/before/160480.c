void RenderFrameHostImpl::ResetWaitingState() {
  DCHECK(is_active());

  if (is_waiting_for_beforeunload_ack_) {
    is_waiting_for_beforeunload_ack_ = false;
    if (beforeunload_timeout_)
      beforeunload_timeout_->Stop();
  }
  send_before_unload_start_time_ = base::TimeTicks();
  render_view_host_->is_waiting_for_close_ack_ = false;
  network_service_connection_error_handler_holder_.reset();
}
