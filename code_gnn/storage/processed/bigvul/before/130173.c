void RenderFrameHostImpl::InsertCSS(const std::string& css) {
  Send(new FrameMsg_CSSInsertRequest(routing_id_, css));
}
