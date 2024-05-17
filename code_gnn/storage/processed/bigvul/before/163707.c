void WebContentsImpl::NotifyFindReply(int request_id,
                                      int number_of_matches,
                                      const gfx::Rect& selection_rect,
                                      int active_match_ordinal,
                                      bool final_update) {
  if (delegate_ && !is_being_destroyed_ &&
      !GetRenderProcessHost()->FastShutdownStarted()) {
    delegate_->FindReply(this, request_id, number_of_matches, selection_rect,
                         active_match_ordinal, final_update);
  }
}
