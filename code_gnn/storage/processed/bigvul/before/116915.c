  Browser* ProcessSessionWindows(std::vector<SessionWindow*>* windows) {
    base::TimeDelta time_to_process_sessions =
        base::TimeTicks::Now() - restore_started_;
    UMA_HISTOGRAM_CUSTOM_TIMES(
        "SessionRestore.TimeToProcessSessions",
        time_to_process_sessions,
        base::TimeDelta::FromMilliseconds(10),
        base::TimeDelta::FromSeconds(1000),
        100);

    if (windows->empty()) {
      return FinishedTabCreation(false, false);
    }

#if defined(OS_CHROMEOS)
    chromeos::BootTimesLoader::Get()->AddLoginTimeMarker(
        "SessionRestore-CreatingTabs-Start", false);
#endif
    StartTabCreation();

    Browser* current_browser =
        browser_ ? browser_ : BrowserList::GetLastActiveWithProfile(profile_);
    Browser* last_browser = NULL;
    bool has_tabbed_browser = false;

    bool has_visible_browser = false;
    for (std::vector<SessionWindow*>::iterator i = windows->begin();
         i != windows->end(); ++i) {
      if ((*i)->show_state != ui::SHOW_STATE_MINIMIZED)
        has_visible_browser = true;
    }

    for (std::vector<SessionWindow*>::iterator i = windows->begin();
         i != windows->end(); ++i) {
      Browser* browser = NULL;
      if (!has_tabbed_browser && (*i)->type == Browser::TYPE_TABBED)
        has_tabbed_browser = true;
      if (i == windows->begin() && (*i)->type == Browser::TYPE_TABBED &&
          current_browser && current_browser->is_type_tabbed() &&
          !current_browser->profile()->IsOffTheRecord()) {
        browser = current_browser;
      } else {
#if defined(OS_CHROMEOS)
    chromeos::BootTimesLoader::Get()->AddLoginTimeMarker(
        "SessionRestore-CreateRestoredBrowser-Start", false);
#endif
        ui::WindowShowState show_state = (*i)->show_state;
        if (!has_visible_browser) {
          show_state = ui::SHOW_STATE_NORMAL;
          has_visible_browser = true;
        }

        browser = CreateRestoredBrowser(
            static_cast<Browser::Type>((*i)->type), (*i)->bounds, show_state);
#if defined(OS_CHROMEOS)
    chromeos::BootTimesLoader::Get()->AddLoginTimeMarker(
        "SessionRestore-CreateRestoredBrowser-End", false);
#endif
      }
      if ((*i)->type == Browser::TYPE_TABBED)
        last_browser = browser;
      WebContents* active_tab = browser->GetSelectedWebContents();
      int initial_tab_count = browser->tab_count();
      int selected_tab_index = (*i)->selected_tab_index;
      RestoreTabsToBrowser(*(*i), browser, selected_tab_index);
      ShowBrowser(browser, initial_tab_count, selected_tab_index);
      if (clobber_existing_tab_ && i == windows->begin() &&
          (*i)->type == Browser::TYPE_TABBED && active_tab &&
          browser == browser_ && browser->tab_count() > initial_tab_count) {
        browser->CloseTabContents(active_tab);
        active_tab = NULL;
      }
      tab_loader_->TabIsLoading(
          &browser->GetSelectedWebContents()->GetController());
      NotifySessionServiceOfRestoredTabs(browser, initial_tab_count);
    }

    if (last_browser && !urls_to_open_.empty())
      AppendURLsToBrowser(last_browser, urls_to_open_);
#if defined(OS_CHROMEOS)
    chromeos::BootTimesLoader::Get()->AddLoginTimeMarker(
        "SessionRestore-CreatingTabs-End", false);
#endif
    Browser* finished_browser = FinishedTabCreation(true, has_tabbed_browser);
    if (finished_browser)
      last_browser = finished_browser;
    return last_browser;
  }
