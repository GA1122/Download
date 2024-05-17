bool ContentSecurityPolicy::AllowAncestors(
    LocalFrame* frame,
    const KURL& url,
    SecurityViolationReportingPolicy reporting_policy) const {
  bool is_allowed = true;
  for (const auto& policy : policies_)
    is_allowed &= policy->AllowAncestors(frame, url, reporting_policy);
  return is_allowed;
}
