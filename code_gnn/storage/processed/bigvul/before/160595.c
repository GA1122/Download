void RenderFrameImpl::DidStartLoading(bool to_different_document) {
  TRACE_EVENT1("navigation,rail", "RenderFrameImpl::didStartLoading",
               "id", routing_id_);
  render_view_->FrameDidStartLoading(frame_);

  if (!to_different_document)
    Send(new FrameHostMsg_DidStartLoading(routing_id_, to_different_document));
}
