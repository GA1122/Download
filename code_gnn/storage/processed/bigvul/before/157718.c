void WebContentsImpl::AttachInterstitialPage(
    InterstitialPageImpl* interstitial_page) {
  DCHECK(!interstitial_page_ && interstitial_page);
  interstitial_page_ = interstitial_page;

  CancelActiveAndPendingDialogs();

  for (auto& observer : observers_)
    observer.DidAttachInterstitialPage();

  if (frame_tree_.IsLoading())
    LoadingStateChanged(true, true, nullptr);

  if (node_.OuterContentsFrameTreeNode()) {
    if (GetRenderManager()->GetProxyToOuterDelegate()) {
      DCHECK(
          static_cast<RenderWidgetHostViewBase*>(interstitial_page->GetView())
              ->IsRenderWidgetHostViewChildFrame());
      RenderWidgetHostViewChildFrame* view =
          static_cast<RenderWidgetHostViewChildFrame*>(
              interstitial_page->GetView());
      GetRenderManager()->SetRWHViewForInnerContents(view);
    }
  }

#if defined(OS_ANDROID)
  static_cast<RenderFrameHostImpl*>(interstitial_page_->GetMainFrame())
      ->GetRenderWidgetHost()
      ->SetImportance(GetMainFrame()->GetRenderWidgetHost()->importance());
#endif

  if (accessibility_mode_.has_mode(ui::AXMode::kNativeAPIs)) {
    RenderFrameHostImpl* rfh =
        static_cast<RenderFrameHostImpl*>(GetMainFrame());
    if (rfh) {
      BrowserAccessibilityManager* accessibility_manager =
          rfh->browser_accessibility_manager();
      if (accessibility_manager)
        accessibility_manager->set_hidden_by_interstitial_page(true);
    }
    rfh = static_cast<RenderFrameHostImpl*>(
        GetInterstitialPage()->GetMainFrame());
    if (rfh) {
      BrowserAccessibilityManager* accessibility_manager =
          rfh->GetOrCreateBrowserAccessibilityManager();
      if (accessibility_manager)
        accessibility_manager->OnWindowFocused();
    }
  }
}
