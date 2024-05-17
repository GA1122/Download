RenderFrameHostImpl::TakeNavigationRequestForCommit(
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params) {
  NavigationHandleImpl* navigation_handle =
      navigation_request_ ? navigation_request_->navigation_handle() : nullptr;

  if (navigation_handle && navigation_handle->GetURL() == params.url) {
    return std::move(navigation_request_);
  }


  NavigationEntryImpl* entry_for_request = nullptr;
  bool is_renderer_initiated = true;

  if (navigation_handle) {
    NavigationEntryImpl* pending_entry =
        NavigationEntryImpl::FromNavigationEntry(
            frame_tree_node()->navigator()->GetController()->GetPendingEntry());
    bool pending_entry_matches_handle =
        pending_entry && pending_entry->GetUniqueID() ==
                             navigation_handle->pending_nav_entry_id();
    if (navigation_handle->GetURL() == params.base_url &&
        pending_entry_matches_handle &&
        !pending_entry->GetBaseURLForDataURL().is_empty()) {
      entry_for_request = pending_entry;
      is_renderer_initiated = pending_entry->is_renderer_initiated();
    }
  }

  return NavigationRequest::CreateForCommit(
      frame_tree_node_, entry_for_request, params,
      is_renderer_initiated  ,
      false  );
}
