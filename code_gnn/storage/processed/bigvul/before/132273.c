void RenderFrameImpl::UpdateEncoding(WebFrame* frame,
                                     const std::string& encoding_name) {
  if (!frame->parent())
    Send(new FrameHostMsg_UpdateEncoding(routing_id_, encoding_name));
}
