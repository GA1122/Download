void NavigationControllerImpl::CopyStateFromAndPrune(
    NavigationController* temp) {
  CHECK(CanPruneAllButVisible());

  NavigationControllerImpl* source =
      static_cast<NavigationControllerImpl*>(temp);
  NavigationEntryImpl* last_committed =
      NavigationEntryImpl::FromNavigationEntry(GetLastCommittedEntry());
  scoped_refptr<SiteInstance> site_instance(
      last_committed->site_instance());
  int32 minimum_page_id = last_committed->GetPageID();
  int32 max_page_id =
      web_contents_->GetMaxPageIDForSiteInstance(site_instance.get());

  PruneAllButVisibleInternal();

  DCHECK_EQ(1, GetEntryCount());
  source->PruneOldestEntryIfFull();

  int max_source_index = source->last_committed_entry_index_;
  if (max_source_index == -1)
    max_source_index = source->GetEntryCount();
  else
    max_source_index++;
  InsertEntriesFrom(*source, max_source_index);

  last_committed_entry_index_ = GetEntryCount() - 1;

  web_contents_->SetHistoryLengthAndPrune(site_instance.get(),
                                          max_source_index,
                                          minimum_page_id);

  web_contents_->CopyMaxPageIDsFrom(source->web_contents());

  if (max_page_id > -1) {
    web_contents_->UpdateMaxPageIDForSiteInstance(site_instance.get(),
                                                  max_page_id);
  }
}
