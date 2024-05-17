  bool WebContentsImpl::IsLoading() const {
    return frame_tree_.IsLoading() &&
         !(ShowingInterstitialPage() &&
           GetRenderManager()->interstitial_page()->pause_throbber());
//          !(ShowingInterstitialPage() && interstitial_page_->pause_throbber());
  }