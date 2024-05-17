SyncedTabDelegate* ExtractSyncedTabDelegate(const NotificationSource& source) {
  TabContentsWrapper* tab =  TabContentsWrapper::GetCurrentWrapperForContents(
      Source<NavigationController>(source).ptr()->tab_contents());
  if (!tab)
    return NULL;
  return tab->synced_tab_delegate();
}
