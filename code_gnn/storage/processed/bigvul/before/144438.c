void WebContentsImpl::AttachInterstitialPage(
    InterstitialPageImpl* interstitial_page) {
  DCHECK(interstitial_page);
  GetRenderManager()->set_interstitial_page(interstitial_page);

  CancelActiveAndPendingDialogs();

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidAttachInterstitialPage());
}
