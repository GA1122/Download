void RenderWidgetHostImpl::OnMsgHasTouchEventHandlers(bool has_handlers) {
  if (has_touch_handler_ == has_handlers)
    return;
  has_touch_handler_ = has_handlers;
  if (!has_touch_handler_)
    touch_event_queue_->FlushQueue();
#if defined(OS_ANDROID)
  if (view_)
    view_->HasTouchEventHandlers(has_touch_handler_);
#endif
}
