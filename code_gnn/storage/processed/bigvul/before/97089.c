  wstring GetTabTitle() {
    wstring title;
    EXPECT_TRUE(tab_->GetTabTitle(&title));
    return title;
  }
