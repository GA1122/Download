void WebFrameLoaderClient::detachedFromParent3() {
  webframe_->frame()->script()->proxy()->clearForClose();

  webframe_->drop_client();
}
