void RenderWidgetHostImpl::OnForwardCompositorProto(
    const std::vector<uint8_t>& proto) {
  if (delegate_)
    delegate_->ForwardCompositorProto(this, proto);
}
