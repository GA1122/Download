void RenderViewImpl::reportFindInPageSelection(int request_id,
                                               int active_match_ordinal,
                                               const WebRect& selection_rect) {
#if defined(OS_ANDROID)
  if (synchronous_find_reply_message_.get()) {
    synchronous_find_active_match_ordinal_ = active_match_ordinal;
    return;
  }
#endif

  SendFindReply(request_id,
                -1,
                active_match_ordinal,
                selection_rect,
                false);
}
