void RenderFrameImpl::OnActivateNearestFindResult(int request_id,
                                                  float x,
                                                  float y) {
  WebRect selection_rect;
  int ordinal =
      frame_->SelectNearestFindMatch(WebFloatPoint(x, y), &selection_rect);
  if (ordinal == -1) {
    frame_->IncreaseMatchCount(0, request_id);
    return;
  }

  SendFindReply(request_id, -1  , ordinal, selection_rect,
                true  );
}
