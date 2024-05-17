void WebContentsImpl::WasHidden() {
  if (capturer_count_ == 0) {
    RenderWidgetHostViewPort* rwhv =
        RenderWidgetHostViewPort::FromRWHV(GetRenderWidgetHostView());
    if (rwhv)
      rwhv->Hide();
  }

  FOR_EACH_OBSERVER(WebContentsObserver, observers_, WasHidden());

  should_normally_be_visible_ = false;
}
