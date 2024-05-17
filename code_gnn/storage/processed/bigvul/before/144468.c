void WebContentsImpl::DetachInterstitialPage() {
  if (ShowingInterstitialPage())
    GetRenderManager()->remove_interstitial_page();
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidDetachInterstitialPage());
}
