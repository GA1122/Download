void RenderViewImpl::HandleInputEvent(
    const blink::WebCoalescedInputEvent& input_event,
    const ui::LatencyInfo& latency_info,
    HandledEventCallback callback) {
  if (is_swapped_out_) {
    std::move(callback).Run(INPUT_EVENT_ACK_STATE_NOT_CONSUMED, latency_info,
                            nullptr, base::nullopt);
    return;
  }
  idle_user_detector_->ActivityDetected();
  RenderWidget::HandleInputEvent(input_event, latency_info,
                                 std::move(callback));
}
