void RenderFrameImpl::ShowDeferredContextMenu(const ContextMenuParams& params) {
  Send(new FrameHostMsg_ContextMenu(routing_id_, params));
}
