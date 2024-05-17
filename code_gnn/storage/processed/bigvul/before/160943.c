ChromeClientImpl* ChromeClientImpl::Create(WebViewImpl* web_view) {
  return new ChromeClientImpl(web_view);
}
