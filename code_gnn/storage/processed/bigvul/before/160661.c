void RenderFrameImpl::OnFindMatchRects(int current_version) {
  std::vector<gfx::RectF> match_rects;

  int rects_version = frame_->FindMatchMarkersVersion();
  if (current_version != rects_version) {
    WebVector<WebFloatRect> web_match_rects;
    frame_->FindMatchRects(web_match_rects);
    match_rects.reserve(web_match_rects.size());
    for (size_t i = 0; i < web_match_rects.size(); ++i)
      match_rects.push_back(gfx::RectF(web_match_rects[i]));
  }

  gfx::RectF active_rect = frame_->ActiveFindMatchRect();
  Send(new FrameHostMsg_FindMatchRects_Reply(routing_id_, rects_version,
                                             match_rects, active_rect));
}
