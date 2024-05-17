void WebContentsImpl::WasOccluded() {
  if (!IsBeingCaptured()) {
    for (RenderWidgetHostView* view : GetRenderWidgetHostViewsInTree())
      view->WasOccluded();
  }

  should_normally_be_occluded_ = true;
}
