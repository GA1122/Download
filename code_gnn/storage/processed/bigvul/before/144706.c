void WebContentsImpl::WasHidden() {
  if (capturer_count_ == 0) {
    for (RenderWidgetHostView* view : GetRenderWidgetHostViewsInTree()) {
      if (view)
        view->Hide();
    }
  }

  FOR_EACH_OBSERVER(WebContentsObserver, observers_, WasHidden());

  should_normally_be_visible_ = false;
}
