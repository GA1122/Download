void WebFrameLoaderClient::documentElementAvailable() {
  if (webframe_->client())
    webframe_->client()->didCreateDocumentElement(webframe_);
}
