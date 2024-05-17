void SessionModelAssociator::PopulateSessionSpecificsWindow(
    const SessionWindow& window,
    sync_pb::SessionWindow* session_window) {
  DCHECK(CalledOnValidThread());
  session_window->set_window_id(window.window_id.id());
  session_window->set_selected_tab_index(window.selected_tab_index);
  if (window.type == Browser::TYPE_TABBED) {
    session_window->set_browser_type(
      sync_pb::SessionWindow_BrowserType_TYPE_TABBED);
  } else if (window.type == Browser::TYPE_POPUP) {
    session_window->set_browser_type(
      sync_pb::SessionWindow_BrowserType_TYPE_POPUP);
  } else {
    LOG(WARNING) << "Session Sync unable to handle windows of type" <<
        window.type;
    return;
  }
  for (std::vector<SessionTab*>::const_iterator i = window.tabs.begin();
      i != window.tabs.end(); ++i) {
    const SessionTab* tab = *i;
    if (!IsValidSessionTab(*tab))
      continue;
    session_window->add_tab(tab->tab_id.id());
  }
}
