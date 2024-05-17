void WebFrameLoaderClient::dispatchDidFinishDocumentLoad() {
  webframe_->ClearPasswordListeners();

  if (webframe_->client())
    webframe_->client()->didFinishDocumentLoad(webframe_);
}
