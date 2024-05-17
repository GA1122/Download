void RenderViewHostImpl::LostCapture() {
  RenderWidgetHostImpl::LostCapture();
  delegate_->LostCapture();
}
