void Browser::OpenFileManager() {
  UserMetrics::RecordAction(UserMetricsAction("OpenFileManager"));
  ShowSingletonTabRespectRef(GURL(chrome::kChromeUIFileManagerURL));
}
