  void SetDefaultUIExpectations(MockWebsiteSettingsUI* mock_ui) {
    EXPECT_CALL(*mock_ui, SetPermissionInfo(_));
    EXPECT_CALL(*mock_ui, SetIdentityInfo(_));
    EXPECT_CALL(*mock_ui, SetCookieInfo(_));
  }
