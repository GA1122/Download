bool WebContentsImpl::HasMouseLock(RenderWidgetHostImpl* render_widget_host) {
  return mouse_lock_widget_ == render_widget_host &&
         GetTopLevelRenderWidgetHostView()->IsMouseLocked();
}
