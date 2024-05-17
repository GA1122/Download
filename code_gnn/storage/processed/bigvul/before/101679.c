void Browser::GoBack(WindowOpenDisposition disposition) {
  UserMetrics::RecordAction(UserMetricsAction("Back"));

  TabContentsWrapper* current_tab = GetSelectedTabContentsWrapper();
  if (CanGoBack()) {
    TabContents* new_tab = GetOrCloneTabForDisposition(disposition);
    if (current_tab->tab_contents()->showing_interstitial_page() &&
        (new_tab != current_tab->tab_contents()))
      return;
    new_tab->controller().GoBack();
  }
}
