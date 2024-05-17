bool RenderFrameHostImpl::NavigationRequestWasIntendedForPendingEntry(
    NavigationRequest* request,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params,
    bool same_document) {
  NavigationEntryImpl* pending_entry = NavigationEntryImpl::FromNavigationEntry(
      frame_tree_node()->navigator()->GetController()->GetPendingEntry());
  if (!pending_entry)
    return false;
  if (request->nav_entry_id() != pending_entry->GetUniqueID())
    return false;
  if (!same_document) {
    if (request->common_params().url != params.base_url ||
        pending_entry->GetBaseURLForDataURL().is_empty()) {
      return false;
    }
  }
  return true;
}
