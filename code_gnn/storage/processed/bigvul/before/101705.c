void Browser::NewTab() {
  UserMetrics::RecordAction(UserMetricsAction("NewTab"));

  if (is_type_tabbed()) {
    AddBlankTab(true);
    GetSelectedTabContentsWrapper()->view()->RestoreFocus();
  } else {
    Browser* b = GetOrCreateTabbedBrowser(profile_);
    b->AddBlankTab(true);
    b->window()->Show();
    b->GetSelectedTabContentsWrapper()->view()->RestoreFocus();
  }
}
