  void CheckEmbargo(const GURL& url,
                    ContentSettingsType permission,
                    ContentSetting expected_setting) {
    EXPECT_EQ(expected_setting,
              autoblocker_->GetEmbargoResult(url, permission).content_setting);
  }
