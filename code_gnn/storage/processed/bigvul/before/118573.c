AppShortcutManager::AppShortcutManager(Profile* profile)
    : profile_(profile),
      is_profile_info_cache_observer_(false),
      prefs_(profile->GetPrefs()) {
  DCHECK(!content::BrowserThread::IsThreadInitialized(
             content::BrowserThread::UI) ||
         content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));

  registrar_.Add(this,
                 chrome::NOTIFICATION_EXTENSION_INSTALLED_DEPRECATED,
                 content::Source<Profile>(profile_));
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_UNINSTALLED,
                 content::Source<Profile>(profile_));
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSIONS_READY,
                 content::Source<Profile>(profile_));

  ProfileManager* profile_manager = g_browser_process->profile_manager();
  if (profile_manager) {
    profile_manager->GetProfileInfoCache().AddObserver(this);
    is_profile_info_cache_observer_ = true;
  }
}
