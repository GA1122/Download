void NavigationController::CopyStateFromAndPrune(NavigationController* source) {
  NavigationEntry* last_committed = GetLastCommittedEntry();
  SiteInstance* site_instance =
      last_committed ? last_committed->site_instance() : NULL;
  int32 minimum_page_id = last_committed ? last_committed->page_id() : -1;

  DCHECK((transient_entry_index_ != -1 &&
          transient_entry_index_ == entry_count() - 1) ||
         (pending_entry_ && (pending_entry_index_ == -1 ||
                             pending_entry_index_ == entry_count() - 1)) ||
         (!pending_entry_ && last_committed_entry_index_ == entry_count() - 1));

  PruneAllButActive();

  int max_source_index = source->pending_entry_index_ != -1 ?
      source->pending_entry_index_ : source->last_committed_entry_index_;
  if (max_source_index == -1)
    max_source_index = source->entry_count();
  else
    max_source_index++;
  InsertEntriesFrom(*source, max_source_index);

  last_committed_entry_index_ = entry_count() - 1;
  if (pending_entry_index_ != -1)
    pending_entry_index_ = entry_count() - 1;
  if (transient_entry_index_ != -1) {
    transient_entry_index_ = entry_count() - 1;
    if (last_committed_entry_index_ != -1)
      last_committed_entry_index_--;
  }

  tab_contents_->SetHistoryLengthAndPrune(site_instance,
                                          max_source_index,
                                          minimum_page_id);
}
