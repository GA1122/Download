void ContentSecurityPolicy::copyPluginTypesFrom(
    const ContentSecurityPolicy* other) {
  for (const auto& policy : other->m_policies) {
    if (policy->hasPluginTypes()) {
      addAndReportPolicyFromHeaderValue(policy->pluginTypesText(),
                                        policy->headerType(),
                                        policy->headerSource());
    }
  }
}
