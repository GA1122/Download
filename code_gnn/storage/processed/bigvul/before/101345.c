void SessionModelAssociator::PopulateSessionSpecificsTab(
    const SessionTab& tab,
    sync_pb::SessionTab* session_tab) {
  DCHECK(CalledOnValidThread());
  session_tab->set_tab_id(tab.tab_id.id());
  session_tab->set_window_id(tab.window_id.id());
  session_tab->set_tab_visual_index(tab.tab_visual_index);
  session_tab->set_current_navigation_index(
      tab.current_navigation_index);
  session_tab->set_pinned(tab.pinned);
  session_tab->set_extension_app_id(tab.extension_app_id);
  for (std::vector<TabNavigation>::const_iterator i =
      tab.navigations.begin(); i != tab.navigations.end(); ++i) {
    const TabNavigation navigation = *i;
    sync_pb::TabNavigation* tab_navigation =
        session_tab->add_navigation();
    PopulateSessionSpecificsNavigation(&navigation, tab_navigation);
  }
}
