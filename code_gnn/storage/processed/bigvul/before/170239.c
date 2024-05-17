  void ListOfURLs() {
    base::ListValue urls;
    for (size_t i = 0; i < arraysize(kRestoredURLs); ++i) {
      urls.AppendString(kRestoredURLs[i]);
      expected_urls_.push_back(GURL(kRestoredURLs[i]));
    }
    PolicyMap policies;
    policies.Set(
        key::kRestoreOnStartup, POLICY_LEVEL_MANDATORY, POLICY_SCOPE_USER,
        POLICY_SOURCE_CLOUD,
        base::WrapUnique(new base::Value(SessionStartupPref::kPrefValueURLs)),
        nullptr);
    policies.Set(key::kRestoreOnStartupURLs, POLICY_LEVEL_MANDATORY,
                 POLICY_SCOPE_USER, POLICY_SOURCE_CLOUD, urls.CreateDeepCopy(),
                 nullptr);
    provider_.UpdateChromePolicy(policies);
  }
