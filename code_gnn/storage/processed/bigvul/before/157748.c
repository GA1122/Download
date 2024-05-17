void WebContentsImpl::DetachInterstitialPage(bool has_focus) {
  bool interstitial_pausing_throbber =
      ShowingInterstitialPage() && interstitial_page_->pause_throbber();
  if (ShowingInterstitialPage())
    interstitial_page_ = nullptr;

  RenderFrameHostImpl* rfh = GetMainFrame();
  if (rfh) {
    BrowserAccessibilityManager* accessibility_manager =
        rfh->browser_accessibility_manager();
    if (accessibility_manager)
      accessibility_manager->set_hidden_by_interstitial_page(false);
  }

  if (has_focus && GetRenderViewHost()->GetWidget()->GetView())
    GetRenderViewHost()->GetWidget()->GetView()->Focus();

  for (auto& observer : observers_)
    observer.DidDetachInterstitialPage();

  if (node_.OuterContentsFrameTreeNode()) {
    if (GetRenderManager()->GetProxyToOuterDelegate()) {
      DCHECK(static_cast<RenderWidgetHostViewBase*>(
                 GetRenderManager()->current_frame_host()->GetView())
                 ->IsRenderWidgetHostViewChildFrame());
      RenderWidgetHostViewChildFrame* view =
          static_cast<RenderWidgetHostViewChildFrame*>(
              GetRenderManager()->current_frame_host()->GetView());
      GetRenderManager()->SetRWHViewForInnerContents(view);
    }
  }

  if (interstitial_pausing_throbber && frame_tree_.IsLoading())
    LoadingStateChanged(true, true, nullptr);
}
