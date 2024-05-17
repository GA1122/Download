  void DownloadFilesCheckErrorsSetup() {
    embedded_test_server()->ServeFilesFromDirectory(GetTestDataDirectory());
    ASSERT_TRUE(embedded_test_server()->Start());
    std::vector<DownloadItem*> download_items;
    GetDownloads(browser(), &download_items);
    ASSERT_TRUE(download_items.empty());

    EnableFileChooser(true);
  }
