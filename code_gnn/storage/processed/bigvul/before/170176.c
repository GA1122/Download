  void ApplySafeSearchPolicy(std::unique_ptr<base::Value> legacy_safe_search,
                             std::unique_ptr<base::Value> google_safe_search,
                             std::unique_ptr<base::Value> legacy_youtube,
                             std::unique_ptr<base::Value> youtube_restrict) {
    PolicyMap policies;
    SetPolicy(&policies, key::kForceSafeSearch, std::move(legacy_safe_search));
    SetPolicy(&policies, key::kForceGoogleSafeSearch,
              std::move(google_safe_search));
    SetPolicy(&policies, key::kForceYouTubeSafetyMode,
              std::move(legacy_youtube));
    SetPolicy(&policies, key::kForceYouTubeRestrict,
              std::move(youtube_restrict));
    UpdateProviderPolicy(policies);
  }
