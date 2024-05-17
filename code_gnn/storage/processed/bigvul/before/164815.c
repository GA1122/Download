  void CheckDownloadStatesForBrowser(Browser* browser,
                                     size_t num,
                                     DownloadItem::DownloadState state) {
    std::vector<DownloadItem*> download_items;
    GetDownloads(browser, &download_items);

    EXPECT_EQ(num, download_items.size());

    for (size_t i = 0; i < download_items.size(); ++i) {
      EXPECT_EQ(state, download_items[i]->GetState()) << " Item " << i;
    }
  }
