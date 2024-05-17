TabContents* Browser::AddRestoredTab(
    const std::vector<TabNavigation>& navigations,
    int tab_index,
    int selected_navigation,
    const std::string& extension_app_id,
    bool select,
    bool pin,
    bool from_last_session,
    SessionStorageNamespace* session_storage_namespace) {
  GURL restore_url = navigations.at(selected_navigation).virtual_url();
  TabContentsWrapper* wrapper = TabContentsFactory(
      profile(),
      tab_util::GetSiteInstanceForNewTab(NULL, profile_, restore_url),
      MSG_ROUTING_NONE,
      GetSelectedTabContents(),
      session_storage_namespace);
  TabContents* new_tab = wrapper->tab_contents();
  wrapper->extension_tab_helper()->SetExtensionAppById(extension_app_id);
  std::vector<NavigationEntry*> entries;
  TabNavigation::CreateNavigationEntriesFromTabNavigations(
      profile_, navigations, &entries);
  new_tab->controller().Restore(
      selected_navigation, from_last_session, &entries);
  DCHECK_EQ(0u, entries.size());

  int add_types = select ? TabStripModel::ADD_ACTIVE :
      TabStripModel::ADD_NONE;
  if (pin) {
    tab_index = std::min(tab_index, tabstrip_model()->IndexOfFirstNonMiniTab());
    add_types |= TabStripModel::ADD_PINNED;
  }
  tab_handler_->GetTabStripModel()->InsertTabContentsAt(tab_index, wrapper,
                                                        add_types);
  if (select) {
    window_->Activate();
  } else {
    new_tab->view()->SizeContents(window_->GetRestoredBounds().size());
    new_tab->HideContents();
  }
  SessionService* session_service =
      SessionServiceFactory::GetForProfileIfExisting(profile_);
  if (session_service)
    session_service->TabRestored(wrapper, pin);
  return new_tab;
}
