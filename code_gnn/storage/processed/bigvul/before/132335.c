void RenderFrameImpl::registerProtocolHandler(const WebString& scheme,
                                              const WebURL& url,
                                              const WebString& title) {
  bool user_gesture = WebUserGestureIndicator::isProcessingUserGesture();
  Send(new FrameHostMsg_RegisterProtocolHandler(
      routing_id_,
      base::UTF16ToUTF8(base::StringPiece16(scheme)),
      url,
      title,
      user_gesture));
}
