void RenderFrameHostImpl::DeleteSurroundingText(size_t before, size_t after) {
  Send(new InputMsg_DeleteSurroundingText(routing_id_, before, after));
}
