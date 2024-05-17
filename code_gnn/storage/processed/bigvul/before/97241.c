void WebFrameLoaderClient::startDownload(const ResourceRequest& request) {
  if (webframe_->client()) {
    WrappedResourceRequest webreq(request);
    webframe_->client()->loadURLExternally(
        webframe_, webreq, WebKit::WebNavigationPolicyDownload);
  }
}
