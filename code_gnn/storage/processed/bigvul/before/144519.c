blink::WebDisplayMode WebContentsImpl::GetDisplayMode(
    RenderWidgetHostImpl* render_widget_host) const {
  if (!RenderViewHostImpl::From(render_widget_host))
    return blink::WebDisplayModeBrowser;

  return delegate_ ? delegate_->GetDisplayMode(this)
                   : blink::WebDisplayModeBrowser;
}
