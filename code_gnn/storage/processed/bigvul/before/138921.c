bool ShouldUseCustomizedDefaultWallpaper() {
  PrefService* pref_service = g_browser_process->local_state();

  return !(pref_service->FindPreference(
      prefs::kCustomizationDefaultWallpaperURL)->IsDefaultValue());
}
