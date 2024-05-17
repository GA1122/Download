  void AppendURLsToBrowser(Browser* browser,
                           const std::vector<GURL>& urls) {
    for (size_t i = 0; i < urls.size(); ++i) {
      int add_types = TabStripModel::ADD_FORCE_INDEX;
      if (i == 0)
        add_types |= TabStripModel::ADD_ACTIVE;
      int index = browser->GetIndexForInsertionDuringRestore(i);
      browser::NavigateParams params(browser, urls[i],
                                     content::PAGE_TRANSITION_START_PAGE);
      params.disposition = i == 0 ? NEW_FOREGROUND_TAB : NEW_BACKGROUND_TAB;
      params.tabstrip_index = index;
      params.tabstrip_add_types = add_types;
      browser::Navigate(&params);
    }
  }
