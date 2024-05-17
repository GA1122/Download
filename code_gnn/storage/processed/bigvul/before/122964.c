void RenderWidgetHostImpl::ForwardTouchEvent(
    const WebKit::WebTouchEvent& touch_event) {
  touch_event_queue_->QueueEvent(touch_event);
}
