void AutomationProviderDownloadModelChangedObserver::OnDownloadRemoved(
    DownloadManager* manager, DownloadItem* item) {
  ModelChanged();
}
