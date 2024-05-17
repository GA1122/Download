void SessionService::BuildCommandsForBrowser(
    Browser* browser,
    std::vector<SessionCommand*>* commands,
    IdToRange* tab_to_available_range,
    std::set<SessionID::id_type>* windows_to_track) {
  DCHECK(browser && commands);
  DCHECK(browser->session_id().id());

  ui::WindowShowState show_state = ui::SHOW_STATE_NORMAL;
  if (browser->window()->IsMaximized())
    show_state = ui::SHOW_STATE_MAXIMIZED;
  else if (browser->window()->IsMinimized())
    show_state = ui::SHOW_STATE_MINIMIZED;

  commands->push_back(
      CreateSetWindowBoundsCommand(browser->session_id(),
                                   browser->window()->GetRestoredBounds(),
                                   show_state));

  commands->push_back(CreateSetWindowTypeCommand(
      browser->session_id(), WindowTypeForBrowserType(browser->type())));

  bool added_to_windows_to_track = false;
  for (int i = 0; i < browser->tab_count(); ++i) {
    TabContentsWrapper* tab = browser->GetTabContentsWrapperAt(i);
    DCHECK(tab);
    if (tab->profile() == profile() || profile() == NULL) {
      BuildCommandsForTab(browser->session_id(), tab, i,
                          browser->IsTabPinned(i),
                          commands, tab_to_available_range);
      if (windows_to_track && !added_to_windows_to_track) {
        windows_to_track->insert(browser->session_id().id());
        added_to_windows_to_track = true;
      }
    }
  }
  commands->push_back(
      CreateSetSelectedTabInWindow(browser->session_id(),
                                   browser->active_index()));
}
