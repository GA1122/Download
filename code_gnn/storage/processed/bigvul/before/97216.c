bool WebFrameLoaderClient::hasWebView() const {
  return webframe_->GetWebViewImpl() != NULL;
}
