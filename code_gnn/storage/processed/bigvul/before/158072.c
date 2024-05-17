void LocalFrameClientImpl::DispatchDidFinishDocumentLoad() {
  if (web_frame_->Client())
    web_frame_->Client()->DidFinishDocumentLoad();
}
