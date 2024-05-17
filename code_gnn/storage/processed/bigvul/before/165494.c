bool ContentSecurityPolicy::AllowRequestWithoutIntegrity(
    mojom::RequestContextType context,
    const KURL& url,
    RedirectStatus redirect_status,
    SecurityViolationReportingPolicy reporting_policy,
    CheckHeaderType check_header_type) const {
  for (const auto& policy : policies_) {
    if (CheckHeaderTypeMatches(check_header_type, policy->HeaderType()) &&
        !policy->AllowRequestWithoutIntegrity(context, url, redirect_status,
                                              reporting_policy))
      return false;
  }
  return true;
}
