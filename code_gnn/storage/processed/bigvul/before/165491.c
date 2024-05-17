bool ContentSecurityPolicy::AllowPluginType(
    const String& type,
    const String& type_attribute,
    const KURL& url,
    SecurityViolationReportingPolicy reporting_policy) const {
  for (const auto& policy : policies_) {
    if (!policy->AllowPluginType(type, type_attribute, url, reporting_policy))
      return false;
  }
  return true;
}
