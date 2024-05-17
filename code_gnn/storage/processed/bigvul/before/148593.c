void WebContentsImpl::WasHidden() {
  if (capturer_count_ == 0) {
    for (RenderWidgetHostView* view : GetRenderWidgetHostViewsInTree())
      view->Hide();

    SendPageMessage(new PageMsg_WasHidden(MSG_ROUTING_NONE));
  }

  last_hidden_time_ = base::TimeTicks::Now();

  for (auto& observer : observers_)
    observer.WasHidden();

  should_normally_be_visible_ = false;
}
