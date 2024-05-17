void ContentSecurityPolicy::ReportMixedContent(
    const KURL& mixed_url,
    RedirectStatus redirect_status) const {
  for (const auto& policy : policies_)
    policy->ReportMixedContent(mixed_url, redirect_status);
}
