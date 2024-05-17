void Browser::FindReplyHelper(TabContents* tab,
                              int request_id,
                              int number_of_matches,
                              const gfx::Rect& selection_rect,
                              int active_match_ordinal,
                              bool final_update) {
  TabContentsWrapper* tcw = TabContentsWrapper::GetCurrentWrapperForContents(
      tab);
  if (!tcw || !tcw->find_tab_helper())
    return;

  tcw->find_tab_helper()->HandleFindReply(request_id, number_of_matches,
                                          selection_rect, active_match_ordinal,
                                          final_update);
}
