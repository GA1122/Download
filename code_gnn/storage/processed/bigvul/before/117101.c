void SessionService::SortTabsBasedOnVisualOrderAndPrune(
    std::map<int, SessionWindow*>* windows,
    std::vector<SessionWindow*>* valid_windows) {
  std::map<int, SessionWindow*>::iterator i = windows->begin();
  while (i != windows->end()) {
    if (i->second->tabs.empty() || i->second->is_constrained ||
        !should_track_changes_for_browser_type(
            static_cast<Browser::Type>(i->second->type))) {
      delete i->second;
      windows->erase(i++);
    } else {
      std::sort(i->second->tabs.begin(), i->second->tabs.end(),
                &TabVisualIndexSortFunction);
      if (valid_windows->empty()) {
        valid_windows->push_back(i->second);
      } else {
        valid_windows->insert(
            std::upper_bound(valid_windows->begin(), valid_windows->end(),
                             i->second, &WindowOrderSortFunction),
            i->second);
      }
      ++i;
    }
  }
}
