Frame* LocalFrameClientImpl::Parent() const {
  return ToCoreFrame(web_frame_->Parent());
}
