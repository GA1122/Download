  void EnsureFindBoxOpenForBrowser(Browser* browser) {
    browser->ShowFindBar();
    gfx::Point position;
    bool fully_visible = false;
    EXPECT_TRUE(GetFindBarWindowInfoForBrowser(
                    browser, &position, &fully_visible));
    EXPECT_TRUE(fully_visible);
  }
