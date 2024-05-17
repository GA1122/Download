void LocalFrameClientImpl::DidBlockFramebust(const KURL& url) {
  web_frame_->Client()->DidBlockFramebust(url);
}
