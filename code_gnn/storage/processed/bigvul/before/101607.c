void Browser::ConvertPopupToTabbedBrowser() {
  UserMetrics::RecordAction(UserMetricsAction("ShowAsTab"));
  int tab_strip_index = tab_handler_->GetTabStripModel()->active_index();
  TabContentsWrapper* contents =
      tab_handler_->GetTabStripModel()->DetachTabContentsAt(tab_strip_index);
  Browser* browser = Browser::Create(profile_);
  browser->tabstrip_model()->AppendTabContents(contents, true);
  browser->window()->Show();
}
