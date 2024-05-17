void RenderWidgetHostImpl::QueueSyntheticGesture(
    std::unique_ptr<SyntheticGesture> synthetic_gesture,
    base::OnceCallback<void(SyntheticGesture::Result)> on_complete) {
  if (!synthetic_gesture_controller_ && view_) {
    synthetic_gesture_controller_ =
        std::make_unique<SyntheticGestureController>(
            this, view_->CreateSyntheticGestureTarget());
  }
  if (synthetic_gesture_controller_) {
    synthetic_gesture_controller_->QueueSyntheticGesture(
        std::move(synthetic_gesture), std::move(on_complete));
  }
}
