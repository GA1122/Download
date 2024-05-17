void WebContentsImpl::ForwardCompositorProto(
    RenderWidgetHostImpl* render_widget_host,
    const std::vector<uint8_t>& proto) {
  if (render_widget_host != GetRenderViewHost()->GetWidget())
    return;

  if (delegate_)
    delegate_->ForwardCompositorProto(proto);
}
