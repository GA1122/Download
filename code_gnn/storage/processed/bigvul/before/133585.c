bool WebContentsImpl::IsActiveEntry(int32 page_id) {
  NavigationEntryImpl* visible_entry =
      NavigationEntryImpl::FromNavigationEntry(controller_.GetVisibleEntry());
  return (visible_entry != NULL &&
          visible_entry->site_instance() == GetSiteInstance() &&
          visible_entry->GetPageID() == page_id);
}
