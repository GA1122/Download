void WebContentsImpl::WasShown() {
  const Visibility previous_visibility = GetVisibility();

  controller_.SetActive(true);

  if (auto* view = GetRenderWidgetHostView()) {
    view->Show();
#if defined(OS_MACOSX)
    view->SetActive(true);
#endif
  }

  if (!ShowingInterstitialPage())
    SetVisibilityForChildViews(true);

  SendPageMessage(new PageMsg_WasShown(MSG_ROUTING_NONE));

  last_active_time_ = base::TimeTicks::Now();
  should_normally_be_visible_ = true;
  NotifyVisibilityChanged(previous_visibility);
}
