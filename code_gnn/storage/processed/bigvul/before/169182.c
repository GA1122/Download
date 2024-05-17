RenderFrameHostImpl::TakeNavigationHandleForCommit(
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params) {
  NavigationHandleImpl* navigation_handle =
      navigation_request_ ? navigation_request_->navigation_handle() : nullptr;

  if (navigation_handle && navigation_handle->GetURL() == params.url) {
    std::unique_ptr<NavigationHandleImpl> result_navigation_handle =
        navigation_request()->TakeNavigationHandle();
    navigation_request_.reset();
    return result_navigation_handle;
  }

  int entry_id_for_data_nav = 0;
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
      entry_id_for_data_nav = navigation_handle->pending_nav_entry_id();
      is_renderer_initiated = pending_entry->is_renderer_initiated();
    }
  }

  return NavigationHandleImpl::Create(
      params.url, params.redirects, frame_tree_node_, is_renderer_initiated,
      false  , base::TimeTicks::Now(),
      entry_id_for_data_nav,
      false,                   
      CSPDisposition::CHECK,   
      false,                   
      base::nullopt,           
      nullptr);                
}
