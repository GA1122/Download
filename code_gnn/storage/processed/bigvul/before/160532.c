void WebContentsImpl::WasHidden() {
  if (!IsBeingCaptured()) {
    if (auto* view = GetRenderWidgetHostView())
      view->Hide();

    if (!ShowingInterstitialPage())
      SetVisibilityForChildViews(false);

    SendPageMessage(new PageMsg_WasHidden(MSG_ROUTING_NONE));
  }

  for (auto& observer : observers_)
    observer.WasHidden();

  should_normally_be_visible_ = false;
}
