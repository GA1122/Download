RenderFrameHostImpl::TakeNavigationHandleForCommit(
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params) {
  bool is_browser_initiated = (params.nav_entry_id != 0);

  NavigationHandleImpl* navigation_handle = GetNavigationHandle();

  if (params.was_within_same_document) {
    if (is_browser_initiated && navigation_handle &&
        navigation_handle->IsSameDocument() &&
        navigation_handle->GetURL() == params.url) {
      std::unique_ptr<NavigationHandleImpl> result_navigation_handle =
          navigation_request()->TakeNavigationHandle();
      navigation_request_.reset();
      return result_navigation_handle;
    }

    bool is_renderer_initiated = true;
    int pending_nav_entry_id = 0;
    NavigationEntryImpl* pending_entry =
        NavigationEntryImpl::FromNavigationEntry(
            frame_tree_node()->navigator()->GetController()->GetPendingEntry());
    if (pending_entry && pending_entry->GetUniqueID() == params.nav_entry_id) {
      pending_nav_entry_id = params.nav_entry_id;
      is_renderer_initiated = pending_entry->is_renderer_initiated();
    }

    return NavigationHandleImpl::Create(
        params.url, params.redirects, frame_tree_node_, is_renderer_initiated,
        params.was_within_same_document, base::TimeTicks::Now(),
        pending_nav_entry_id,
        false,                   
        CSPDisposition::CHECK,   
        false,                   
        base::nullopt);          
  }

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
      params.was_within_same_document, base::TimeTicks::Now(),
      entry_id_for_data_nav,
      false,                   
      CSPDisposition::CHECK,   
      false,                   
      base::nullopt);          
}
