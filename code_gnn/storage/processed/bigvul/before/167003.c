NavigationType NavigationControllerImpl::ClassifyNavigation(
    RenderFrameHostImpl* rfh,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params) const {
  if (params.did_create_new_entry) {
    if (!rfh->GetParent()) {
      return NAVIGATION_TYPE_NEW_PAGE;
    }

    if (!GetLastCommittedEntry())
      return NAVIGATION_TYPE_NAV_IGNORE;

    return NAVIGATION_TYPE_NEW_SUBFRAME;
  }

  DCHECK(!params.history_list_was_cleared);

  if (rfh->GetParent()) {
    if (GetLastCommittedEntry()) {
      return NAVIGATION_TYPE_AUTO_SUBFRAME;
    } else {
      return NAVIGATION_TYPE_NAV_IGNORE;
    }
  }

  if (params.nav_entry_id == 0) {

    NavigationEntry* last_committed = GetLastCommittedEntry();
    if (!last_committed)
      return NAVIGATION_TYPE_NAV_IGNORE;

    return NAVIGATION_TYPE_EXISTING_PAGE;
  }

  if (pending_entry_ && pending_entry_index_ == -1 &&
      pending_entry_->GetUniqueID() == params.nav_entry_id) {
    if (!GetLastCommittedEntry() ||
        GetLastCommittedEntry()->site_instance() != rfh->GetSiteInstance())
      return NAVIGATION_TYPE_NEW_PAGE;

    return NAVIGATION_TYPE_SAME_PAGE;
  }

  if (!GetLastCommittedEntry())
    return NAVIGATION_TYPE_NEW_PAGE;

  if (params.intended_as_new_entry) {
    return NAVIGATION_TYPE_EXISTING_PAGE;
  }

  if (params.url_is_unreachable && failed_pending_entry_id_ != 0 &&
      params.nav_entry_id == failed_pending_entry_id_) {
    return NAVIGATION_TYPE_EXISTING_PAGE;
  }

  int existing_entry_index = GetEntryIndexWithUniqueID(params.nav_entry_id);
  if (existing_entry_index == -1) {
    return NAVIGATION_TYPE_NEW_PAGE;
  }

  return NAVIGATION_TYPE_EXISTING_PAGE;
}
