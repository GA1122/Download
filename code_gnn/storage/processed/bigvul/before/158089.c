void LocalFrameClientImpl::FrameRectsChanged(const IntRect& frame_rect) {
  DCHECK(web_frame_->Client());
  web_frame_->Client()->FrameRectsChanged(frame_rect);
}
