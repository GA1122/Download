blink::WebPushClient* RenderFrameImpl::PushClient() {
  if (!push_messaging_client_)
    push_messaging_client_ = new PushMessagingClient(this);
  return push_messaging_client_;
}
