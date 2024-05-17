void TestingAutomationProvider::GetDownloadDirectory(
    int handle, FilePath* download_directory) {
  if (tab_tracker_->ContainsHandle(handle)) {
    NavigationController* tab = tab_tracker_->GetResource(handle);
    DownloadManager* dlm = tab->GetBrowserContext()->GetDownloadManager();
    *download_directory =
        DownloadPrefs::FromDownloadManager(dlm)->download_path();
  }
}
