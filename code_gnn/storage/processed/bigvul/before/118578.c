AppShortcutManager::~AppShortcutManager() {
  if (g_browser_process && is_profile_info_cache_observer_) {
    ProfileManager* profile_manager = g_browser_process->profile_manager();
    if (profile_manager)
      profile_manager->GetProfileInfoCache().RemoveObserver(this);
  }
}
