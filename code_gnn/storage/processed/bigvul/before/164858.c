  bool RunSizeTest(Browser* browser,
                   SizeTestType type,
                   const std::string& partial_indication,
                   const std::string& total_indication) {
    embedded_test_server()->ServeFilesFromDirectory(GetTestDataDirectory());
    embedded_test_server()->RegisterRequestHandler(base::Bind(
        &content::SlowDownloadHttpResponse::HandleSlowDownloadRequest));
    EXPECT_TRUE(embedded_test_server()->Start());

    base::ScopedAllowBlockingForTesting allow_blocking;
    EXPECT_TRUE(type == SIZE_TEST_TYPE_UNKNOWN || type == SIZE_TEST_TYPE_KNOWN);
    if (type != SIZE_TEST_TYPE_KNOWN && type != SIZE_TEST_TYPE_UNKNOWN)
      return false;
    GURL url(type == SIZE_TEST_TYPE_KNOWN
                 ? embedded_test_server()->GetURL(
                       content::SlowDownloadHttpResponse::kKnownSizeUrl)
                 : embedded_test_server()->GetURL(
                       content::SlowDownloadHttpResponse::kUnknownSizeUrl));
    GURL finish_url = embedded_test_server()->GetURL(
        content::SlowDownloadHttpResponse::kFinishDownloadUrl);

    base::FilePath filename;
    net::FileURLToFilePath(url, &filename);
    base::string16 expected_title_in_progress(
        base::ASCIIToUTF16(partial_indication) + filename.LossyDisplayName());
    base::string16 expected_title_finished(
        base::ASCIIToUTF16(total_indication) + filename.LossyDisplayName());

    std::unique_ptr<content::DownloadTestObserver> observer(
        CreateWaiter(browser, 1));
    ui_test_utils::NavigateToURL(browser, url);



    ui_test_utils::NavigateToURLWithDisposition(
        browser, finish_url, WindowOpenDisposition::NEW_FOREGROUND_TAB,
        ui_test_utils::BROWSER_TEST_WAIT_FOR_NAVIGATION);
    observer->WaitForFinished();
    EXPECT_EQ(1u, observer->NumDownloadsSeenInState(DownloadItem::COMPLETE));
    CheckDownloadStatesForBrowser(browser, 1, DownloadItem::COMPLETE);

    EXPECT_EQ(2, browser->tab_strip_model()->count());


    base::FilePath basefilename(filename.BaseName());
    net::FileURLToFilePath(url, &filename);
    base::FilePath download_path =
        GetDownloadDirectory(browser).Append(basefilename);

    bool downloaded_path_exists = base::PathExists(download_path);
    EXPECT_TRUE(downloaded_path_exists);
    if (!downloaded_path_exists)
      return false;

    size_t file_size = content::SlowDownloadHttpResponse::kFirstDownloadSize +
                       content::SlowDownloadHttpResponse::kSecondDownloadSize;
    std::string expected_contents(file_size, '*');
    EXPECT_TRUE(VerifyFile(download_path, expected_contents, file_size));

    EXPECT_TRUE(base::DieFileDie(download_path, false));
    EXPECT_FALSE(base::PathExists(download_path));

    return true;
  }
