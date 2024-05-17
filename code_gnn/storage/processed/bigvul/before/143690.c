void RenderWidgetHostImpl::QueueSyntheticGesture(
    scoped_ptr<SyntheticGesture> synthetic_gesture,
    const base::Callback<void(SyntheticGesture::Result)>& on_complete) {
  if (!synthetic_gesture_controller_ && view_) {
    synthetic_gesture_controller_.reset(
        new SyntheticGestureController(view_->CreateSyntheticGestureTarget()));
  }
  if (synthetic_gesture_controller_) {
    synthetic_gesture_controller_->QueueSyntheticGesture(
        std::move(synthetic_gesture), on_complete);
  }
}
