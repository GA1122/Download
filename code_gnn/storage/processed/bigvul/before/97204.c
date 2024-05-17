void WebFrameLoaderClient::dispatchWillClose() {
  if (webframe_->client())
    webframe_->client()->willClose(webframe_);
}
