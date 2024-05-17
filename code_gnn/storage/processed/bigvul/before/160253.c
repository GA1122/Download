  void GrantNotificationPermissionForTest(const GURL& url) {
    GURL origin = url.GetOrigin();
    DesktopNotificationProfileUtil::GrantPermission(profile(), origin);
    ASSERT_EQ(CONTENT_SETTING_ALLOW,
              PermissionManager::Get(profile())
                  ->GetPermissionStatus(CONTENT_SETTINGS_TYPE_NOTIFICATIONS,
                                        origin, origin)
                  .content_setting);
  }
