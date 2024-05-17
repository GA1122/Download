void RenderFrameImpl::didStartLoading(bool to_different_document) {
  TRACE_EVENT1("navigation", "RenderFrameImpl::didStartLoading",
               "id", routing_id_);
  render_view_->FrameDidStartLoading(frame_);
  Send(new FrameHostMsg_DidStartLoading(routing_id_, to_different_document));
}
