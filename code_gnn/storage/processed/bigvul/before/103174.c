void Browser::RegisterUserPrefs(PrefService* prefs) {
  prefs->RegisterStringPref(prefs::kHomePage,
                            chrome::kChromeUINewTabURL);
  prefs->RegisterBooleanPref(prefs::kHomePageIsNewTabPage, true);
  prefs->RegisterBooleanPref(prefs::kShowHomeButton, false);
#if defined(OS_MACOSX)
  prefs->RegisterBooleanPref(prefs::kConfirmToQuitEnabled, false);
  prefs->RegisterBooleanPref(prefs::kShowUpdatePromotionInfoBar, true);
#endif
  prefs->RegisterBooleanPref(prefs::kDeleteBrowsingHistory, true);
  prefs->RegisterBooleanPref(prefs::kDeleteDownloadHistory, true);
  prefs->RegisterBooleanPref(prefs::kDeleteCache, true);
  prefs->RegisterBooleanPref(prefs::kDeleteCookies, true);
  prefs->RegisterBooleanPref(prefs::kDeletePasswords, false);
  prefs->RegisterBooleanPref(prefs::kDeleteFormData, false);
  prefs->RegisterIntegerPref(prefs::kDeleteTimePeriod, 0);
  prefs->RegisterBooleanPref(prefs::kCheckDefaultBrowser, true);
  prefs->RegisterBooleanPref(prefs::kShowOmniboxSearchHint, true);
  prefs->RegisterBooleanPref(prefs::kWebAppCreateOnDesktop, true);
  prefs->RegisterBooleanPref(prefs::kWebAppCreateInAppsMenu, true);
  prefs->RegisterBooleanPref(prefs::kWebAppCreateInQuickLaunchBar, true);
  prefs->RegisterBooleanPref(prefs::kUseVerticalTabs, false);
  prefs->RegisterBooleanPref(prefs::kEnableTranslate, true);
  prefs->RegisterBooleanPref(prefs::kEnableBookmarkBar, true);
  prefs->RegisterBooleanPref(prefs::kRemotingHasSetupCompleted, false);
  prefs->RegisterStringPref(prefs::kCloudPrintEmail, std::string());
  prefs->RegisterBooleanPref(prefs::kCloudPrintProxyEnabled, true);
  prefs->RegisterBooleanPref(prefs::kDevToolsDisabled, false);
  prefs->RegisterBooleanPref(prefs::kIncognitoEnabled, true);
  prefs->RegisterIntegerPref(prefs::kDevToolsSplitLocation, -1);
  prefs->RegisterDictionaryPref(prefs::kBrowserWindowPlacement);
  prefs->RegisterDictionaryPref(prefs::kPreferencesWindowPlacement);
  prefs->RegisterBooleanPref(prefs::kDisable3DAPIs, false);
  prefs->RegisterBooleanPref(prefs::kPluginsAllowOutdated, false);
  prefs->RegisterBooleanPref(prefs::kEnableHyperlinkAuditing, true);
  prefs->RegisterBooleanPref(prefs::kEnableReferrers, true);
}