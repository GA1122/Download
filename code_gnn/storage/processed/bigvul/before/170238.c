  void Last() {
    PolicyMap policies;
    policies.Set(
        key::kRestoreOnStartup, POLICY_LEVEL_MANDATORY, POLICY_SCOPE_USER,
        POLICY_SOURCE_CLOUD,
        base::WrapUnique(new base::Value(SessionStartupPref::kPrefValueLast)),
        nullptr);
    provider_.UpdateChromePolicy(policies);
    for (size_t i = 0; i < arraysize(kRestoredURLs); ++i)
      expected_urls_.push_back(GURL(kRestoredURLs[i]));
  }
