void Browser::ShowDownloadsTab() {
  UserMetrics::RecordAction(UserMetricsAction("ShowDownloads"));
#if !defined(OS_CHROMEOS)
  if (window()) {
    DownloadShelf* shelf = window()->GetDownloadShelf();
    if (shelf->IsShowing())
      shelf->Close();
  }
#endif
  ShowSingletonTabOverwritingNTP(
      GetSingletonTabNavigateParams(GURL(chrome::kChromeUIDownloadsURL)));
}
