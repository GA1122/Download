bool LocalFrameClientImpl::ShouldTrackUseCounter(const KURL& url) {
  if (web_frame_->Client())
    return web_frame_->Client()->ShouldTrackUseCounter(url);
  return false;
}
