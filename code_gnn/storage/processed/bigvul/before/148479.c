void WebContentsImpl::OnFindReply(RenderFrameHostImpl* source,
                                  int request_id,
                                  int number_of_matches,
                                  const gfx::Rect& selection_rect,
                                  int active_match_ordinal,
                                  bool final_update) {
  if (active_match_ordinal > 0)
    SetFocusedFrame(source->frame_tree_node(), source->GetSiteInstance());

  GetOrCreateFindRequestManager()->OnFindReply(
      source, request_id, number_of_matches, selection_rect,
      active_match_ordinal, final_update);
}
