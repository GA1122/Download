void Browser::ShowDownloadsTab() {
  UserMetrics::RecordAction(UserMetricsAction("ShowDownloads"), profile_);
  if (window()) {
    DownloadShelf* shelf = window()->GetDownloadShelf();
    if (shelf->IsShowing())
      shelf->Close();
  }
  ShowSingletonTab(GURL(chrome::kChromeUIDownloadsURL));
}
