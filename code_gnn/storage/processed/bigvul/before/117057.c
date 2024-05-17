void SessionService::BuildCommandsFromBrowsers(
    std::vector<SessionCommand*>* commands,
    IdToRange* tab_to_available_range,
    std::set<SessionID::id_type>* windows_to_track) {
  DCHECK(commands);
  for (BrowserList::const_iterator i = BrowserList::begin();
       i != BrowserList::end(); ++i) {
    if (should_track_changes_for_browser_type((*i)->type()) &&
        (*i)->tab_count() && (*i)->window()) {
      BuildCommandsForBrowser(*i, commands, tab_to_available_range,
                              windows_to_track);
    }
  }
}
