  void Blocked() {
    PolicyMap policies;
    policies.Set(
        key::kRestoreOnStartup, POLICY_LEVEL_MANDATORY, POLICY_SCOPE_USER,
        POLICY_SOURCE_CLOUD,
        std::make_unique<base::Value>(SessionStartupPref::kPrefValueLast),
        nullptr);
    auto urls = std::make_unique<base::Value>(base::Value::Type::LIST);
    for (const auto* url_string : kRestoredURLs)
      urls->GetList().emplace_back(url_string);
    policies.Set(key::kURLBlacklist, POLICY_LEVEL_MANDATORY, POLICY_SCOPE_USER,
                 POLICY_SOURCE_CLOUD, std::move(urls), nullptr);
    provider_.UpdateChromePolicy(policies);
    blocked_ = true;
    for (size_t i = 0; i < arraysize(kRestoredURLs); ++i)
      expected_urls_.emplace_back(kRestoredURLs[i]);
  }
