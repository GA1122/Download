blink::WebDisplayMode WebContentsImpl::GetDisplayMode(
    RenderWidgetHostImpl* render_widget_host) const {
  if (!RenderViewHostImpl::From(render_widget_host))
    return blink::kWebDisplayModeBrowser;

  return delegate_ ? delegate_->GetDisplayMode(this)
                   : blink::kWebDisplayModeBrowser;
}
