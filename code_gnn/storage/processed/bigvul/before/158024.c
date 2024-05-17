LocalFrameClientImpl::CreateApplicationCacheHost(
    WebApplicationCacheHostClient* client) {
  if (!web_frame_->Client())
    return nullptr;
  return web_frame_->Client()->CreateApplicationCacheHost(client);
}
