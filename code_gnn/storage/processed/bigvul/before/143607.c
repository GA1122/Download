void RenderWidgetHostImpl::DidFlush() {
  if (synthetic_gesture_controller_)
    synthetic_gesture_controller_->OnDidFlushInput();
}
