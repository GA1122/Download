void WebFrameLoaderClient::assignIdentifierToInitialRequest(
    unsigned long identifier, DocumentLoader* loader,
    const ResourceRequest& request) {
  if (webframe_->client()) {
    WrappedResourceRequest webreq(request);
    webframe_->client()->assignIdentifierToRequest(
        webframe_, identifier, webreq);
  }
}
