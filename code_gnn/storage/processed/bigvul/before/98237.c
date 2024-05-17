DownloadManager& DownloadManager::shared()
{
    DEFINE_STATIC_LOCAL(DownloadManager, downloadManager, ());
    return downloadManager;
}
