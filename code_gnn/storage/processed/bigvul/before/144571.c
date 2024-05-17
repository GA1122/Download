bool WebContentsImpl::IsFullscreenForCurrentTab(
    RenderWidgetHostImpl* render_widget_host) const {
  if (!RenderViewHostImpl::From(render_widget_host))
    return false;

  return delegate_ ? delegate_->IsFullscreenForTabOrPending(this) : false;
}
