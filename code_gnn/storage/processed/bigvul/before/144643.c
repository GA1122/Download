void WebContentsImpl::RendererResponsive(
    RenderWidgetHostImpl* render_widget_host) {
  if (render_widget_host != GetRenderViewHost()->GetWidget())
    return;

  if (delegate_)
    delegate_->RendererResponsive(this);
}
