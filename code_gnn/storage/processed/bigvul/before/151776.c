  explicit TestInterstitialPage(WebContents* tab) {
    base::FilePath file_path;
    bool success = PathService::Get(chrome::DIR_TEST_DATA, &file_path);
    EXPECT_TRUE(success);
    file_path = file_path.AppendASCII("focus/typical_page.html");
    success = base::ReadFileToString(file_path, &html_contents_);
    EXPECT_TRUE(success);
    interstitial_page_ = content::InterstitialPage::Create(
        tab, true, GURL("http://interstitial.com"), this);

    interstitial_page_->Show();
    content::WaitForInterstitialAttach(tab);

    EXPECT_TRUE(tab->ShowingInterstitialPage());
  }
