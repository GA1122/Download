bool RenderWidgetHostImpl::IsInOverscrollGesture() const {
  return overscroll_controller_.get() &&
         overscroll_controller_->overscroll_mode() != OVERSCROLL_NONE;
}
