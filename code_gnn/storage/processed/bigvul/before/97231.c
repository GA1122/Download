void WebFrameLoaderClient::revertToProvisionalState(DocumentLoader*) {
  has_representation_ = true;
}
