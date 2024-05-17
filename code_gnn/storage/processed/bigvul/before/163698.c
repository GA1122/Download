void WebContentsImpl::DetachInterstitialPage(bool has_focus) {
  bool interstitial_pausing_throbber =
      ShowingInterstitialPage() && interstitial_page_->pause_throbber();
  if (ShowingInterstitialPage())
    interstitial_page_ = nullptr;

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
