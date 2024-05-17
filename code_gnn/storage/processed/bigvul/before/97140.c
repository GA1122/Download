void NavigationController::RendererDidNavigateToSamePage(
    const ViewHostMsg_FrameNavigate_Params& params) {
  NavigationEntry* existing_entry = GetEntryWithPageID(
      tab_contents_->GetSiteInstance(),
      params.page_id);

  existing_entry->set_unique_id(pending_entry_->unique_id());

  existing_entry->set_url(params.url);

  DiscardNonCommittedEntries();
}
