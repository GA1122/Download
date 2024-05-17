void Browser::TabClosingAt(TabStripModel* tab_strip_model,
                           TabContentsWrapper* contents,
                           int index) {
  NotificationService::current()->Notify(
      NotificationType::TAB_CLOSING,
      Source<NavigationController>(&contents->controller()),
      NotificationService::NoDetails());

  contents->tab_contents()->set_delegate(NULL);
}
