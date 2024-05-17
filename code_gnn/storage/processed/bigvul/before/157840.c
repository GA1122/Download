void WebContentsImpl::RendererResponsive(
    RenderWidgetHostImpl* render_widget_host) {
  if (delegate_)
    delegate_->RendererResponsive(this, render_widget_host);
}
