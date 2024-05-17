bool ContentSecurityPolicy::AllowWorkerContextFromSource(
    const KURL& url,
    RedirectStatus redirect_status,
    SecurityViolationReportingPolicy reporting_policy,
    CheckHeaderType check_header_type) const {
  if (Document* document = this->GetDocument()) {
    UseCounter::Count(*document, WebFeature::kWorkerSubjectToCSP);
    bool is_allowed_worker = true;
    if (!ShouldBypassContentSecurityPolicy(url, execution_context_)) {
      for (const auto& policy : policies_) {
        if (!CheckHeaderTypeMatches(check_header_type, policy->HeaderType()))
          continue;
        is_allowed_worker &= policy->AllowWorkerFromSource(
            url, redirect_status,
            SecurityViolationReportingPolicy::kSuppressReporting);
      }
    }

    bool is_allowed_script = true;

    if (!ShouldBypassContentSecurityPolicy(url, execution_context_)) {
      for (const auto& policy : policies_) {
        if (!CheckHeaderTypeMatches(check_header_type, policy->HeaderType()))
          continue;
        is_allowed_script &= policy->AllowScriptFromSource(
            url, AtomicString(), IntegrityMetadataSet(), kNotParserInserted,
            redirect_status,
            SecurityViolationReportingPolicy::kSuppressReporting);
      }
    }

    if (is_allowed_worker && !is_allowed_script) {
      UseCounter::Count(*document,
                        WebFeature::kWorkerAllowedByChildBlockedByScript);
    }
  }

  if (ShouldBypassContentSecurityPolicy(url, execution_context_))
    return true;

  bool is_allowed = true;
  for (const auto& policy : policies_) {
    if (!CheckHeaderTypeMatches(check_header_type, policy->HeaderType()))
      continue;
    is_allowed &=
        policy->AllowWorkerFromSource(url, redirect_status, reporting_policy);
  }

  return is_allowed;
}
