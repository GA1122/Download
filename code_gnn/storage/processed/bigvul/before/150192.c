void StartupBrowserCreator::RegisterProfilePrefs(PrefRegistrySimple* registry) {
  registry->RegisterBooleanPref(prefs::kHasSeenWelcomePage, true);
#if defined(OS_WIN) && BUILDFLAG(GOOGLE_CHROME_BRANDING)
  registry->RegisterStringPref(prefs::kNaviOnboardGroup, "");
#endif   
}
