  DownloadItem* CreateSlowTestDownload() {
    std::unique_ptr<content::DownloadTestObserver> observer(
        CreateInProgressDownloadObserver(1));
    embedded_test_server()->RegisterRequestHandler(base::Bind(
        &content::SlowDownloadHttpResponse::HandleSlowDownloadRequest));
    EXPECT_TRUE(embedded_test_server()->Start());
    GURL slow_download_url = embedded_test_server()->GetURL(
        content::SlowDownloadHttpResponse::kKnownSizeUrl);

    DownloadManager* manager = DownloadManagerForBrowser(browser());

    EXPECT_EQ(0, manager->NonMaliciousInProgressCount());
    EXPECT_EQ(0, manager->InProgressCount());
    if (manager->InProgressCount() != 0)
      return NULL;

    ui_test_utils::NavigateToURL(browser(), slow_download_url);

    observer->WaitForFinished();
    EXPECT_EQ(1u, observer->NumDownloadsSeenInState(DownloadItem::IN_PROGRESS));

    DownloadManager::DownloadVector items;
    manager->GetAllDownloads(&items);

    DownloadItem* new_item = NULL;
    for (auto iter = items.begin(); iter != items.end(); ++iter) {
      if ((*iter)->GetState() == DownloadItem::IN_PROGRESS) {
        EXPECT_EQ(NULL, new_item);
        new_item = *iter;
      }
    }
    return new_item;
  }
