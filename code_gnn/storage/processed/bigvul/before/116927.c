  void ShowBrowser(Browser* browser,
                   int initial_tab_count,
                   int selected_session_index) {
    DCHECK(browser);
    DCHECK(browser->tab_count());
    browser->ActivateTabAt(
        std::min(initial_tab_count + std::max(0, selected_session_index),
                 browser->tab_count() - 1), true);

    if (browser_ == browser)
      return;

    browser->window()->Show();
    browser->set_is_session_restore(false);

    browser->GetSelectedWebContents()->GetView()->SetInitialFocus();

    if (!browser_shown_) {
      browser_shown_ = true;
      base::TimeDelta time_to_first_show =
          base::TimeTicks::Now() - restore_started_;
      UMA_HISTOGRAM_CUSTOM_TIMES(
          "SessionRestore.TimeToFirstShow",
          time_to_first_show,
          base::TimeDelta::FromMilliseconds(10),
          base::TimeDelta::FromSeconds(1000),
          100);
    }
  }
