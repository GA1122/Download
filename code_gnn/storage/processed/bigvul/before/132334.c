blink::WebPushClient* RenderFrameImpl::pushClient() {
  if (!push_messaging_dispatcher_)
    push_messaging_dispatcher_ = new PushMessagingDispatcher(this);
  return push_messaging_dispatcher_;
}
