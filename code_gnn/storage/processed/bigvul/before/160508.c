void WebContentsImpl::DoWasUnOccluded() {
  for (RenderWidgetHostView* view : GetRenderWidgetHostViewsInTree())
    view->WasUnOccluded();
}
