void AutomationProviderDownloadModelChangedObserver::OnDownloadCreated(
    DownloadManager* manager, DownloadItem* item) {
  ModelChanged();
}
