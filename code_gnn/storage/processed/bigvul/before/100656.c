void InitializeExtraHeaders(browser::NavigateParams* params,
                            Profile* profile,
                            std::string* extra_headers) {
#if defined(OS_WIN)
#if defined(GOOGLE_CHROME_BUILD)
  if (!profile)
    profile = params->profile;

  if (profile &&
      (params->transition & content::PAGE_TRANSITION_HOME_PAGE) != 0) {
    PrefService* pref_service = profile->GetPrefs();
    if (pref_service) {
      if (!pref_service->GetBoolean(prefs::kHomePageChanged)) {
        std::string homepage = pref_service->GetString(prefs::kHomePage);
        if (homepage == GoogleURLTracker::kDefaultGoogleHomepage) {
          std::wstring rlz_string;
          RLZTracker::GetAccessPointRlz(rlz_lib::CHROME_HOME_PAGE, &rlz_string);
          if (!rlz_string.empty()) {
            net::HttpUtil::AppendHeaderIfMissing("X-Rlz-String",
                                                 WideToUTF8(rlz_string),
                                                 extra_headers);
          }
        }
      }
    }
  }
#endif
#endif
}
