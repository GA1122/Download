void ContentSecurityPolicy::CopyPluginTypesFrom(
    const ContentSecurityPolicy* other) {
  for (const auto& policy : other->policies_) {
    if (policy->HasPluginTypes()) {
      AddAndReportPolicyFromHeaderValue(policy->PluginTypesText(),
                                        policy->HeaderType(),
                                        policy->HeaderSource());
    }
  }
}
