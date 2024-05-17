bool ContentSecurityPolicy::AllowBaseURI(
    const KURL& url,
    RedirectStatus redirect_status,
    SecurityViolationReportingPolicy reporting_policy) const {
  if (ShouldBypassContentSecurityPolicy(url, execution_context_))
    return true;

  bool is_allowed = true;
  for (const auto& policy : policies_) {
    if (!CheckHeaderTypeMatches(CheckHeaderType::kCheckAll,
                                policy->HeaderType()))
      continue;
    is_allowed &= policy->AllowBaseURI(url, redirect_status, reporting_policy);
  }

  return is_allowed;
}
