Frame* LocalFrameClientImpl::NextSibling() const {
  return ToCoreFrame(web_frame_->NextSibling());
}
