void RenderFrameImpl::ReportFindInPageSelection(
    int request_id,
    int active_match_ordinal,
    const blink::WebRect& selection_rect) {
  SendFindReply(request_id, -1  , active_match_ordinal,
                selection_rect, false  );
}
