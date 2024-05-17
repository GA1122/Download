bool WebFrameLoaderClient::dispatchDidLoadResourceFromMemoryCache(
    DocumentLoader* loader,
    const ResourceRequest& request,
    const ResourceResponse& response,
    int length) {
  if (webframe_->client()) {
    WrappedResourceRequest webreq(request);
    WrappedResourceResponse webresp(response);
    webframe_->client()->didLoadResourceFromMemoryCache(
        webframe_, webreq, webresp);
  }
  return false;   
}
