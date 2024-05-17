  void WebContentsImpl::DidProceedOnInterstitial() {
  DCHECK(!(ShowingInterstitialPage() &&
           GetRenderManager()->interstitial_page()->pause_throbber()));
//   DCHECK(!(ShowingInterstitialPage() && interstitial_page_->pause_throbber()));
  
    if (ShowingInterstitialPage() && frame_tree_.IsLoading())
     LoadingStateChanged(true, true, nullptr);
 }