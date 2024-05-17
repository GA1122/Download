WebCookieJar* LocalFrameClientImpl::CookieJar() const {
  if (!web_frame_->Client())
    return nullptr;
  return web_frame_->Client()->CookieJar();
}
