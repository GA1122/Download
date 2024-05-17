blink::WebUserMediaClient* RenderFrameImpl::userMediaClient() {
  if (!web_user_media_client_)
    InitializeUserMediaClient();
  return web_user_media_client_;
}
