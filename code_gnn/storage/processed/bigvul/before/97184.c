void WebFrameLoaderClient::dispatchDidFailLoading(DocumentLoader* loader,
                                                  unsigned long identifier,
                                                  const ResourceError& error) {
  if (webframe_->client()) {
    webframe_->client()->didFailResourceLoad(
        webframe_, identifier, webkit_glue::ResourceErrorToWebURLError(error));
  }
}
