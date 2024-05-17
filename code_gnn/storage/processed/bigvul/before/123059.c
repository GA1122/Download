void RenderWidgetHostImpl::SetIsLoading(bool is_loading) {
  is_loading_ = is_loading;
  gesture_event_filter_->FlingHasBeenHalted();
  if (!view_)
    return;
  view_->SetIsLoading(is_loading);
}
