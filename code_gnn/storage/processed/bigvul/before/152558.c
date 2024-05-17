blink::WebUserMediaClient* RenderFrameImpl::UserMediaClient() {
  if (!web_user_media_client_)
    InitializeUserMediaClient();
  return web_user_media_client_;
}
