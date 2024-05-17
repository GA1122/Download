bool NavigationControllerImpl::RendererDidNavigateAutoSubframe(
    const ViewHostMsg_FrameNavigate_Params& params) {
  DCHECK(GetLastCommittedEntry());

  int entry_index = GetEntryIndexWithPageID(
      web_contents_->GetSiteInstance(),
      params.page_id);
  if (entry_index < 0 ||
      entry_index >= static_cast<int>(entries_.size())) {
    NOTREACHED();
    return false;
  }

  if (entry_index != last_committed_entry_index_) {
    last_committed_entry_index_ = entry_index;
    DiscardNonCommittedEntriesInternal();
    return true;
  }

  return false;
}
