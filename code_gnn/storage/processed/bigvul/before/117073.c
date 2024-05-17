bool SessionService::IsOnlyOneTabLeft() {
  if (!profile()) {
    return false;
  }

  int window_count = 0;
  for (BrowserList::const_iterator i = BrowserList::begin();
       i != BrowserList::end(); ++i) {
    const SessionID::id_type window_id = (*i)->session_id().id();
    if (should_track_changes_for_browser_type((*i)->type()) &&
        (*i)->profile() == profile() &&
        window_closing_ids_.find(window_id) == window_closing_ids_.end()) {
      if (++window_count > 1)
        return false;
      if ((*i)->tab_count() > 0)
        return false;
    }
  }
  return true;
}
