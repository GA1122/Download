void WebContentsImpl::WasShown() {
  controller_.SetActive(true);

  for (RenderWidgetHostView* view : GetRenderWidgetHostViewsInTree()) {
    view->Show();
#if defined(OS_MACOSX)
    view->SetActive(true);
#endif
  }

  SendPageMessage(new PageMsg_WasShown(MSG_ROUTING_NONE));

  last_active_time_ = base::TimeTicks::Now();

  for (auto& observer : observers_)
    observer.WasShown();

  should_normally_be_visible_ = true;
}
