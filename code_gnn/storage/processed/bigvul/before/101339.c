bool SessionModelAssociator::IsValidTab(const SyncedTabDelegate& tab) {
  DCHECK(CalledOnValidThread());
  if ((tab.profile() == sync_service_->profile() ||
       sync_service_->profile() == NULL)) {           
    const SyncedWindowDelegate* window =
        SyncedWindowDelegate::FindSyncedWindowDelegateWithId(
            tab.GetWindowId());
    if (!window)
      return false;
    const NavigationEntry* entry = tab.GetActiveEntry();
    if (!entry)
      return false;
    if (entry->virtual_url().is_valid() &&
        (entry->virtual_url().GetOrigin() != GURL(chrome::kChromeUINewTabURL) ||
         tab.GetEntryCount() > 1)) {
      return true;
    }
  }
  return false;
}
