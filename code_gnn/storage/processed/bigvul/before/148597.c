void WebContentsImpl::WasUnOccluded() {
  for (RenderWidgetHostView* view : GetRenderWidgetHostViewsInTree())
    view->WasUnOccluded();
}
