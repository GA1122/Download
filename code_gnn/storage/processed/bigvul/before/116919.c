  void RestoreForeignSession(
      std::vector<const SessionWindow*>::const_iterator begin,
      std::vector<const SessionWindow*>::const_iterator end) {
    StartTabCreation();
    for (std::vector<const SessionWindow*>::const_iterator i = begin;
         i != end; ++i) {
      Browser* browser = CreateRestoredBrowser(
          static_cast<Browser::Type>((*i)->type),
          (*i)->bounds,
          (*i)->show_state);

      const int initial_tab_count = browser->tab_count();
      int selected_tab_index = (*i)->selected_tab_index;
      RestoreTabsToBrowser(*(*i), browser, selected_tab_index);
      ShowBrowser(browser, initial_tab_count, selected_tab_index);
      tab_loader_->TabIsLoading(
          &browser->GetSelectedWebContents()->GetController());
      NotifySessionServiceOfRestoredTabs(browser, initial_tab_count);
    }

    FinishedTabCreation(true, true);
  }
