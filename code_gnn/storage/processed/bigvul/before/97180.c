void WebFrameLoaderClient::dispatchDidCancelClientRedirect() {
  if (webframe_->client()) {
    expected_client_redirect_src_ = GURL();
    expected_client_redirect_dest_ = GURL();
    webframe_->client()->didCancelClientRedirect(webframe_);
  }

}
