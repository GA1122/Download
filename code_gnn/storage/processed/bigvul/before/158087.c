Frame* LocalFrameClientImpl::FirstChild() const {
  return ToCoreFrame(web_frame_->FirstChild());
}
