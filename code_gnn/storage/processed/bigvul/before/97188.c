void WebFrameLoaderClient::dispatchDidFinishLoading(DocumentLoader* loader,
                                                    unsigned long identifier) {
  if (webframe_->client())
    webframe_->client()->didFinishResourceLoad(webframe_, identifier);
}
