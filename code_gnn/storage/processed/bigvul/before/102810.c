  void CheckAuthenticatedState(TabContents* tab,
                               bool displayed_insecure_content) {
    NavigationEntry* entry = tab->controller().GetActiveEntry();
    ASSERT_TRUE(entry);
    EXPECT_EQ(NORMAL_PAGE, entry->page_type());
    EXPECT_EQ(SECURITY_STYLE_AUTHENTICATED, entry->ssl().security_style());
    EXPECT_EQ(0, entry->ssl().cert_status() & net::CERT_STATUS_ALL_ERRORS);
    EXPECT_EQ(displayed_insecure_content,
              entry->ssl().displayed_insecure_content());
    EXPECT_FALSE(entry->ssl().ran_insecure_content());
  }
