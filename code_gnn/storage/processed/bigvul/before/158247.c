void RenderWidgetHostImpl::DidStopFlinging() {
  is_in_touchpad_gesture_fling_ = false;
  if (view_)
    view_->DidStopFlinging();
}
