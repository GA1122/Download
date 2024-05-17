void NavigationController::RendererDidNavigateToExistingPage(
    const ViewHostMsg_FrameNavigate_Params& params) {
  DCHECK(PageTransition::IsMainFrame(params.transition));

  int entry_index = GetEntryIndexWithPageID(tab_contents_->GetSiteInstance(),
                                            params.page_id);
  DCHECK(entry_index >= 0 &&
         entry_index < static_cast<int>(entries_.size()));
  NavigationEntry* entry = entries_[entry_index].get();

  entry->set_url(params.url);
  DCHECK(entry->site_instance() == NULL ||
         entry->site_instance() == tab_contents_->GetSiteInstance());
  entry->set_site_instance(tab_contents_->GetSiteInstance());

  if (entry == pending_entry_)
    DiscardNonCommittedEntriesInternal();

  last_committed_entry_index_ = entry_index;
}
