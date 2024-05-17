void WebFrameLoaderClient::dispatchDidHandleOnloadEvents() {
  if (webframe_->client())
    webframe_->client()->didHandleOnloadEvents(webframe_);
}
