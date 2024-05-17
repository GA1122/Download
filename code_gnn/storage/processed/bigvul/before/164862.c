  DownloadItem* StartMockDownloadAndInjectError(
      content::TestFileErrorInjector* error_injector,
      download::DownloadInterruptReason error) {
    content::TestFileErrorInjector::FileErrorInfo error_info;
    error_info.code = content::TestFileErrorInjector::FILE_OPERATION_WRITE;
    error_info.operation_instance = 0;
    error_info.error = error;
    error_injector->InjectError(error_info);

    std::unique_ptr<content::DownloadTestObserver> observer(
        new DownloadTestObserverResumable(DownloadManagerForBrowser(browser()),
                                          1));

    if (!embedded_test_server()->Started()) {
      embedded_test_server()->ServeFilesFromDirectory(GetTestDataDirectory());
      EXPECT_TRUE(embedded_test_server()->Start());
    }

    GURL url =
        embedded_test_server()->GetURL("/" + std::string(kDownloadTest1Path));
    ui_test_utils::NavigateToURL(browser(), url);
    observer->WaitForFinished();

    content::DownloadManager::DownloadVector downloads;
    DownloadManagerForBrowser(browser())->GetAllDownloads(&downloads);
    EXPECT_EQ(1u, downloads.size());

    if (downloads.size() != 1)
      return NULL;

    error_injector->ClearError();
    DownloadItem* download = downloads[0];
    EXPECT_EQ(DownloadItem::INTERRUPTED, download->GetState());
    EXPECT_EQ(error, download->GetLastReason());
    return download;
  }
