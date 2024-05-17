void WebContentsImpl::WasShown() {
  controller_.SetActive(true);

  for (RenderWidgetHostView* view : GetRenderWidgetHostViewsInTree()) {
    if (view) {
      view->Show();
#if defined(OS_MACOSX)
      view->SetActive(true);
#endif
    }
  }

  last_active_time_ = base::TimeTicks::Now();

  RenderViewHostImpl* rvh = GetRenderViewHost();
  if (rvh) {
    rvh->GetWidget()->ResizeRectChanged(
        GetRootWindowResizerRect(rvh->GetWidget()));
  }

  FOR_EACH_OBSERVER(WebContentsObserver, observers_, WasShown());

  should_normally_be_visible_ = true;
}
