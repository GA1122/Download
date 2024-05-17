void WebContentsImpl::LostMouseLock(RenderWidgetHostImpl* render_widget_host) {
  if (!RenderViewHostImpl::From(render_widget_host))
    return;

  if (delegate_)
    delegate_->LostMouseLock();
}
