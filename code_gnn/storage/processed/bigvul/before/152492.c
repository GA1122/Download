void RenderFrameImpl::RegisterProtocolHandler(const WebString& scheme,
                                              const WebURL& url,
                                              const WebString& title) {
  bool user_gesture = WebUserGestureIndicator::IsProcessingUserGesture(frame_);
  Send(new FrameHostMsg_RegisterProtocolHandler(routing_id_, scheme.Utf8(), url,
                                                title.Utf16(), user_gesture));
}
