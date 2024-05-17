InterstitialPageImpl::UnderlyingContentObserver::UnderlyingContentObserver(
    WebContents* web_contents,
    InterstitialPageImpl* interstitial)
    : WebContentsObserver(web_contents), interstitial_(interstitial) {
}
