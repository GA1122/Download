RenderWidgetHostImpl* WebContentsImpl::GetMouseLockWidget() {
  if (GetTopLevelRenderWidgetHostView()->IsMouseLocked() ||
      (GetFullscreenRenderWidgetHostView() &&
       GetFullscreenRenderWidgetHostView()->IsMouseLocked()))
    return mouse_lock_widget_;

  return nullptr;
}
