void Browser::TabClosingAt(TabStripModel* tab_strip_model,
                           TabContentsWrapper* contents,
                           int index) {
  NotificationService::current()->Notify(
      content::NOTIFICATION_TAB_CLOSING,
      Source<NavigationController>(&contents->controller()),
      NotificationService::NoDetails());

  SetAsDelegate(contents, NULL);
}
