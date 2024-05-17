void WebContentsImpl::WasShown() {
  controller_.SetActive(true);
  RenderWidgetHostViewPort* rwhv =
      RenderWidgetHostViewPort::FromRWHV(GetRenderWidgetHostView());
  if (rwhv) {
    rwhv->Show();
#if defined(OS_MACOSX)
    rwhv->SetActive(true);
#endif
  }

  last_selected_time_ = base::TimeTicks::Now();

  RenderViewHostImpl* rvh =
      static_cast<RenderViewHostImpl*>(GetRenderViewHost());
  if (rvh) {
    rvh->ResizeRectChanged(GetRootWindowResizerRect());
  }

  FOR_EACH_OBSERVER(WebContentsObserver, observers_, WasShown());

  should_normally_be_visible_ = true;
}
