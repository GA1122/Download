void WebFrameLoaderClient::didDisplayInsecureContent() {
  if (webframe_->client())
    webframe_->client()->didDisplayInsecureContent(webframe_);
}
