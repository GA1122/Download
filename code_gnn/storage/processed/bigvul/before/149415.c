bool ContentSecurityPolicy::allowPluginType(
    const String& type,
    const String& typeAttribute,
    const KURL& url,
    SecurityViolationReportingPolicy reportingPolicy) const {
  for (const auto& policy : m_policies) {
    if (!policy->allowPluginType(type, typeAttribute, url, reportingPolicy))
      return false;
  }
  return true;
}
