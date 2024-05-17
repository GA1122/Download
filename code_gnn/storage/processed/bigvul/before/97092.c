   virtual void SetUp() {
    UITest::SetUp();

    window_ = automation()->GetBrowserWindow(0);
    ASSERT_TRUE(window_.get());

    int active_tab_index = -1;
    ASSERT_TRUE(window_->GetActiveTabIndex(&active_tab_index));
    tab_ = window_->GetTab(active_tab_index);
    ASSERT_TRUE(tab_.get());
  }
