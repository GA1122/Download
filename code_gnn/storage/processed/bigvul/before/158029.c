LocalFrameClientImpl::CreateURLLoaderFactory() {
  return web_frame_->Client()->CreateURLLoaderFactory();
}
