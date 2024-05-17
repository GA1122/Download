void RenderWidgetHostImpl::DetachDelegate() {
  delegate_ = nullptr;
  latency_tracker_.reset_delegate();
}
