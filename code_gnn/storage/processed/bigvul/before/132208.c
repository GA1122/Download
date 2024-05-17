MediaStreamDispatcher* RenderFrameImpl::GetMediaStreamDispatcher() {
  if (!web_user_media_client_)
    InitializeUserMediaClient();
  return web_user_media_client_ ?
      web_user_media_client_->media_stream_dispatcher() : NULL;
}
