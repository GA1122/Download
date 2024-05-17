void Browser::NewTab() {
  UserMetrics::RecordAction(UserMetricsAction("NewTab"), profile_);

  if (type() == TYPE_NORMAL) {
    AddBlankTab(true);
  } else {
    Browser* b = GetOrCreateTabbedBrowser(profile_);
    b->AddBlankTab(true);
    b->window()->Show();
    b->GetSelectedTabContentsWrapper()->view()->RestoreFocus();
  }
}
