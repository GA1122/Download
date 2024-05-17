 void WebContentsImpl::AttachInterstitialPage(
      InterstitialPageImpl* interstitial_page) {
    DCHECK(interstitial_page);
    render_manager_.set_interstitial_page(interstitial_page);
// 
//    
//    
//   if (dialog_manager_)
//     dialog_manager_->CancelActiveAndPendingDialogs(this);
// 
    FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                      DidAttachInterstitialPage());
  }