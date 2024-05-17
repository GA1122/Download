void WebContentsImpl::WasHidden() {
  const Visibility previous_visibility = GetVisibility();

  if (!IsBeingCaptured()) {
    if (auto* view = GetRenderWidgetHostView())
      view->Hide();

    if (!ShowingInterstitialPage())
      SetVisibilityForChildViews(false);

    SendPageMessage(new PageMsg_WasHidden(MSG_ROUTING_NONE));
  }

  should_normally_be_visible_ = false;
  NotifyVisibilityChanged(previous_visibility);
}
