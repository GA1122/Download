void WebContentsImpl::WasHidden() {
  if (!IsBeingCaptured() && !HasPictureInPictureVideo()) {
    if (auto* view = GetRenderWidgetHostView())
      view->Hide();

    if (!ShowingInterstitialPage())
      SetVisibilityForChildViews(false);

    SendPageMessage(new PageMsg_WasHidden(MSG_ROUTING_NONE));
  }

  SetVisibility(Visibility::HIDDEN);
}
