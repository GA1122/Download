void Browser::ReplaceRestoredTab(
    const std::vector<TabNavigation>& navigations,
    int selected_navigation,
    bool from_last_session,
    const std::string& extension_app_id,
    SessionStorageNamespace* session_storage_namespace) {
  GURL restore_url = navigations.at(selected_navigation).virtual_url();
  TabContentsWrapper* wrapper = TabContentsFactory(
      profile(),
      tab_util::GetSiteInstanceForNewTab(NULL, profile_, restore_url),
      MSG_ROUTING_NONE,
      GetSelectedTabContents(),
      session_storage_namespace);
  wrapper->extension_tab_helper()->SetExtensionAppById(extension_app_id);
  TabContents* replacement = wrapper->tab_contents();
  std::vector<NavigationEntry*> entries;
  TabNavigation::CreateNavigationEntriesFromTabNavigations(
      profile_, navigations, &entries);
  replacement->controller().Restore(
      selected_navigation, from_last_session, &entries);
  DCHECK_EQ(0u, entries.size());

  tab_handler_->GetTabStripModel()->ReplaceNavigationControllerAt(
      tab_handler_->GetTabStripModel()->active_index(),
      wrapper);
}
