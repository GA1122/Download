void RenderFrameImpl::ReportFindInPageMatchCount(int request_id,
                                                 int count,
                                                 bool final_update) {
  int active_match_ordinal = count ? -1 : 0;

  SendFindReply(request_id, count, active_match_ordinal, gfx::Rect(),
                final_update);
}
