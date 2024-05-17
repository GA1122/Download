  void GetDownloads(Browser* browser,
                    std::vector<DownloadItem*>* downloads) const {
    DCHECK(downloads);
    DownloadManager* manager = DownloadManagerForBrowser(browser);
    manager->GetAllDownloads(downloads);
  }
