void RenderFrameImpl::unregisterProtocolHandler(const WebString& scheme,
                                                const WebURL& url) {
  bool user_gesture = WebUserGestureIndicator::isProcessingUserGesture();
  Send(new FrameHostMsg_UnregisterProtocolHandler(
      routing_id_,
      base::UTF16ToUTF8(base::StringPiece16(scheme)),
      url,
      user_gesture));
}
