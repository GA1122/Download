void ChromeContentBrowserClient::RegisterLocalStatePrefs(
    PrefRegistrySimple* registry) {
  registry->RegisterStringPref(prefs::kIsolateOrigins, std::string());
  registry->RegisterBooleanPref(prefs::kSitePerProcess, false);
  registry->RegisterBooleanPref(prefs::kTabLifecyclesEnabled, true);
  registry->RegisterBooleanPref(prefs::kWebDriverOverridesIncompatiblePolicies,
                                false);
}
