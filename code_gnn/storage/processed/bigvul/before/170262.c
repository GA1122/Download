  void SetArcEnabledByPolicy(bool enabled) {
    PolicyMap policies;
    policies.Set(key::kArcEnabled, POLICY_LEVEL_MANDATORY, POLICY_SCOPE_USER,
                 POLICY_SOURCE_CLOUD,
                 base::WrapUnique(new base::Value(enabled)), nullptr);
    UpdateProviderPolicy(policies);
    if (browser()) {
      const PrefService* const prefs = browser()->profile()->GetPrefs();
      EXPECT_EQ(prefs->GetBoolean(arc::prefs::kArcEnabled), enabled);
    }
  }
