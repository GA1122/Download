void InterstitialPageImpl::InterstitialPageRVHDelegateView::GotFocus() {
  WebContents* web_contents = interstitial_page_->web_contents();
  if (web_contents)
    static_cast<WebContentsImpl*>(web_contents)->NotifyWebContentsFocused();
}
