  void SetHttpWarningEnabled() {
    scoped_feature_list_.InitAndEnableFeature(
        security_state::kHttpFormWarningFeature);
  }
