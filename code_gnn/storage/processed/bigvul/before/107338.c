void Browser::ReplaceRestoredTab(
    const std::vector<TabNavigation>& navigations,
    int selected_navigation,
    bool from_last_session,
    const std::string& extension_app_id,
    SessionStorageNamespace* session_storage_namespace) {
  TabContentsWrapper* wrapper = TabContentsFactory(profile(), NULL,
      MSG_ROUTING_NONE,
      GetSelectedTabContents(),
      session_storage_namespace);
  TabContents* replacement = wrapper->tab_contents();
  replacement->SetExtensionAppById(extension_app_id);
  replacement->controller().RestoreFromState(navigations, selected_navigation,
                                             from_last_session);

  tab_handler_->GetTabStripModel()->ReplaceNavigationControllerAt(
      tab_handler_->GetTabStripModel()->selected_index(),
      wrapper);
}
