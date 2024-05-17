void WebContentsImpl::WasOccluded() {
  if (!IsBeingCaptured()) {
    for (RenderWidgetHostView* view : GetRenderWidgetHostViewsInTree())
      view->WasOccluded();
  }
  SetVisibility(Visibility::OCCLUDED);
}
