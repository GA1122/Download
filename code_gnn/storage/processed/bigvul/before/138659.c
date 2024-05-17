void RenderFrameHostImpl::DeleteSurroundingTextInCodePoints(int before,
                                                            int after) {
  Send(new InputMsg_DeleteSurroundingTextInCodePoints(routing_id_, before,
                                                      after));
}
