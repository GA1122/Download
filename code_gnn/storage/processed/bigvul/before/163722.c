void WebContentsImpl::WasHidden() {
  if (capturer_count_ == 0) {
    if (auto* view = GetRenderWidgetHostView())
      view->Hide();

    if (!ShowingInterstitialPage())
      SetVisibilityForChildViews(true);

    SendPageMessage(new PageMsg_WasHidden(MSG_ROUTING_NONE));
  }

  last_hidden_time_ = base::TimeTicks::Now();

  for (auto& observer : observers_)
    observer.WasHidden();

  should_normally_be_visible_ = false;
}
