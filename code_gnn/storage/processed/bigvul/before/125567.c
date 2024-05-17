void RenderViewHostImpl::ActivateNearestFindResult(int request_id,
                                                   float x,
                                                   float y) {
  Send(new ViewMsg_ActivateNearestFindResult(GetRoutingID(), request_id, x, y));
}
