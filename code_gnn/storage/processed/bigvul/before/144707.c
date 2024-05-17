void WebContentsImpl::WasOccluded() {
  if (capturer_count_ > 0)
    return;

  for (RenderWidgetHostView* view : GetRenderWidgetHostViewsInTree()) {
    if (view)
      view->WasOccluded();
  }
}
