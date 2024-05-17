void RenderFrameImpl::OnGetNearestFindResult(int nfr_request_id,
                                             float x,
                                             float y) {
  float distance = frame_->DistanceToNearestFindMatch(WebFloatPoint(x, y));
  Send(new FrameHostMsg_GetNearestFindResult_Reply(
      routing_id_, nfr_request_id, distance));
}
