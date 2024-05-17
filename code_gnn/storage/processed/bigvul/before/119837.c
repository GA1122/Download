SiteInstance* GetSiteInstanceFromEntry(NavigationEntry* entry) {
  return NavigationEntryImpl::FromNavigationEntry(entry)->site_instance();
}
