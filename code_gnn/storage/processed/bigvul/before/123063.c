bool RenderWidgetHostImpl::ShouldForwardTouchEvent() const {
  return has_touch_handler_ || !touch_event_queue_->empty();
}
