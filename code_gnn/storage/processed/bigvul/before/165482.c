bool ContentSecurityPolicy::AllowFrameFromSource(
    const KURL& url,
    RedirectStatus redirect_status,
    SecurityViolationReportingPolicy reporting_policy,
    CheckHeaderType check_header_type) const {
  if (ShouldBypassContentSecurityPolicy(url, execution_context_))
    return true;

  bool is_allowed = true;
  for (const auto& policy : policies_) {
    if (!CheckHeaderTypeMatches(check_header_type, policy->HeaderType()))
      continue;
    is_allowed &=
        policy->AllowFrameFromSource(url, redirect_status, reporting_policy);
  }

  return is_allowed;
}
