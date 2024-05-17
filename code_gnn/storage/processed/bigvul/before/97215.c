bool WebFrameLoaderClient::hasFrameView() const {
  return webframe_->GetWebViewImpl() != NULL;
}
