gfx::Rect WebContentsImpl::GetRootWindowResizerRect(
    RenderWidgetHostImpl* render_widget_host) const {
  if (!RenderViewHostImpl::From(render_widget_host))
    return gfx::Rect();

  if (delegate_)
    return delegate_->GetRootWindowResizerRect();
  return gfx::Rect();
}
