void PushMessagingServiceImpl::OnMenuClick() {
#if BUILDFLAG(ENABLE_BACKGROUND)
  chrome::ShowContentSettings(
      BackgroundModeManager::GetBrowserWindowForProfile(profile_),
      CONTENT_SETTINGS_TYPE_NOTIFICATIONS);
#endif   
}
