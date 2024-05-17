bool NavigationControllerImpl::RendererDidNavigateAutoSubframe(
    RenderFrameHostImpl* rfh,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params) {
  DCHECK(ui::PageTransitionCoreTypeIs(params.transition,
                                      ui::PAGE_TRANSITION_AUTO_SUBFRAME));

  DCHECK(GetLastCommittedEntry());

  bool send_commit_notification = false;

  if (params.nav_entry_id) {
    int entry_index = GetEntryIndexWithUniqueID(params.nav_entry_id);
    if (entry_index != -1 && entry_index != last_committed_entry_index_) {
      const GURL& dest_top_url = GetEntryAtIndex(entry_index)->GetURL();
      const GURL& current_top_url = GetLastCommittedEntry()->GetURL();
      if (current_top_url.SchemeIsHTTPOrHTTPS() &&
          dest_top_url.SchemeIsHTTPOrHTTPS() &&
          current_top_url.GetOrigin() != dest_top_url.GetOrigin()) {
        bad_message::ReceivedBadMessage(rfh->GetProcess(),
                                        bad_message::NC_AUTO_SUBFRAME);
      }

      last_committed_entry_index_ = entry_index;
      DiscardNonCommittedEntriesInternal();

      send_commit_notification = true;
    }
  }

  NavigationEntryImpl* last_committed = GetLastCommittedEntry();
  last_committed->AddOrUpdateFrameEntry(
      rfh->frame_tree_node(), params.item_sequence_number,
      params.document_sequence_number, rfh->GetSiteInstance(), nullptr,
      params.url, params.referrer, params.redirects, params.page_state,
      params.method, params.post_id, nullptr  );

  return send_commit_notification;
}
