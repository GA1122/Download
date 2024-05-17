void RenderFrameImpl::PostMessageEvent(FrameMsg_PostMessage_Params params) {
  WebFrame* source_frame = nullptr;
  if (params.source_routing_id != MSG_ROUTING_NONE) {
    RenderFrameProxy* source_proxy =
        RenderFrameProxy::FromRoutingID(params.source_routing_id);
    if (source_proxy)
      source_frame = source_proxy->web_frame();
  }

  WebSecurityOrigin target_origin;
  if (!params.target_origin.empty()) {
    target_origin = WebSecurityOrigin::CreateFromString(
        WebString::FromUTF16(params.target_origin));
  }

  WebDOMMessageEvent msg_event(std::move(params.message->data),
                               WebString::FromUTF16(params.source_origin),
                               source_frame, frame_->GetDocument());

  frame_->DispatchMessageEventWithOriginCheck(
      target_origin, msg_event, params.message->data.has_user_gesture);
}
