void WebContentsImpl::ResizeDueToAutoResize(
    RenderWidgetHostImpl* render_widget_host,
    const gfx::Size& new_size) {
  if (render_widget_host != GetRenderViewHost()->GetWidget())
    return;

  if (delegate_)
    delegate_->ResizeDueToAutoResize(this, new_size);
}
