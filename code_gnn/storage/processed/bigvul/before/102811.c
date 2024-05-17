  void CheckAuthenticationBrokenState(TabContents* tab,
                                      int error,
                                      bool ran_insecure_content,
                                      bool interstitial) {
    NavigationEntry* entry = tab->controller().GetActiveEntry();
    ASSERT_TRUE(entry);
    EXPECT_EQ(interstitial ? INTERSTITIAL_PAGE : NORMAL_PAGE,
              entry->page_type());
    EXPECT_EQ(SECURITY_STYLE_AUTHENTICATION_BROKEN,
              entry->ssl().security_style());
    ASSERT_NE(net::CERT_STATUS_UNABLE_TO_CHECK_REVOCATION, error);
    EXPECT_EQ(error, entry->ssl().cert_status() & net::CERT_STATUS_ALL_ERRORS);
    EXPECT_FALSE(entry->ssl().displayed_insecure_content());
    EXPECT_EQ(ran_insecure_content, entry->ssl().ran_insecure_content());
  }
