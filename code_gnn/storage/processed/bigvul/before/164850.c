  bool InitialSetup() {
    bool have_test_dir =
        base::PathService::Get(chrome::DIR_TEST_DATA, &test_dir_);
    EXPECT_TRUE(have_test_dir);
    if (!have_test_dir)
      return false;

    int window_count = chrome::GetTotalBrowserCount();
    EXPECT_EQ(1, window_count);
    EXPECT_EQ(1, browser()->tab_strip_model()->count());

    browser()->profile()->GetPrefs()->SetBoolean(
        prefs::kPromptForDownload, false);

    DownloadManager* manager = DownloadManagerForBrowser(browser());
    DownloadPrefs::FromDownloadManager(manager)->ResetAutoOpen();

    file_activity_observer_.reset(
        new DownloadTestFileActivityObserver(browser()->profile()));

    return true;
  }
