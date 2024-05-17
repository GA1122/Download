void RenderWidgetHostImpl::RendererExited(base::TerminationStatus status,
                                          int exit_code) {
  if (!renderer_initialized_)
    return;

  renderer_initialized_ = false;

  waiting_for_screen_rects_ack_ = false;

  suppress_next_char_events_ = false;

  ResetSizeAndRepaintPendingFlags();
  current_size_.SetSize(0, 0);
  if (!is_hidden_) {
    process_->WidgetHidden();
    is_hidden_ = true;
  }

  in_flight_event_count_ = 0;
  StopHangMonitorTimeout();

  if (view_) {
    view_->RenderProcessGone(status, exit_code);
    view_.reset();   
  }

  input_router_.reset(new InputRouterImpl(
      process_, this, this, routing_id_, GetInputRouterConfigForPlatform()));

  synthetic_gesture_controller_.reset();
}
