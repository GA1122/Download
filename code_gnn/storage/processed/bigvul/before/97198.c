void WebFrameLoaderClient::dispatchDidReceiveResponse(DocumentLoader* loader,
                                                      unsigned long identifier,
                                                      const ResourceResponse& response) {
  if (webframe_->client()) {
    WrappedResourceResponse webresp(response);
    webframe_->client()->didReceiveResponse(webframe_, identifier, webresp);
  }
}
