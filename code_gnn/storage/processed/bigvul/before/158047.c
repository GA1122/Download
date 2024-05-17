void LocalFrameClientImpl::DidContainInsecureFormAction() {
  if (web_frame_->Client())
    web_frame_->Client()->DidContainInsecureFormAction();
}
