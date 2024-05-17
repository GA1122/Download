void LocalFrameClientImpl::DidRunContentWithCertificateErrors() {
  if (web_frame_->Client())
    web_frame_->Client()->DidRunContentWithCertificateErrors();
}
