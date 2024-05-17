  void RestoreTabsToBrowser(const SessionWindow& window,
                            Browser* browser,
                            int selected_tab_index) {
    DCHECK(!window.tabs.empty());
    int initial_tab_count = browser->tab_count();
    for (std::vector<SessionTab*>::const_iterator i = window.tabs.begin();
         i != window.tabs.end(); ++i) {
      const SessionTab& tab = *(*i);
      const int tab_index = static_cast<int>(i - window.tabs.begin()) +
          initial_tab_count;
      RestoreTab(tab, tab_index, browser,
                 tab_index != (selected_tab_index + initial_tab_count));
    }
  }
