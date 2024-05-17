void Browser::BeforeUnloadFired(TabContents* tab,
                                bool proceed,
                                bool* proceed_to_fire_unload) {
  if (!is_attempting_to_close_browser_) {
    *proceed_to_fire_unload = proceed;
    if (!proceed)
      tab->set_closed_by_user_gesture(false);
    return;
  }

  if (!proceed) {
    CancelWindowClose();
    *proceed_to_fire_unload = false;
    tab->set_closed_by_user_gesture(false);
    return;
  }

  if (RemoveFromSet(&tabs_needing_before_unload_fired_, tab)) {
    tabs_needing_unload_fired_.insert(tab);
    ProcessPendingTabs();
    *proceed_to_fire_unload = false;
    return;
  }

  *proceed_to_fire_unload = true;
}
