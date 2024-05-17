  void TriggerReloadPromptOnClose() const {
    PageInfoBubbleView* const page_info_bubble_view =
        static_cast<PageInfoBubbleView*>(
            PageInfoBubbleView::GetPageInfoBubble());
    ASSERT_NE(nullptr, page_info_bubble_view);

    PageInfoUI::PermissionInfo permission;
    permission.type = ContentSettingsType::CONTENT_SETTINGS_TYPE_NOTIFICATIONS;
    permission.setting = ContentSetting::CONTENT_SETTING_BLOCK;
    permission.default_setting = ContentSetting::CONTENT_SETTING_ASK;
    permission.source = content_settings::SettingSource::SETTING_SOURCE_USER;
    permission.is_incognito = false;
    page_info_bubble_view->OnPermissionChanged(permission);
  }
