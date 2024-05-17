void ContentSecurityPolicy::AddAndReportPolicyFromHeaderValue(
    const String& header,
    ContentSecurityPolicyHeaderType type,
    ContentSecurityPolicyHeaderSource source) {
  wtf_size_t previous_policy_count = policies_.size();
  AddPolicyFromHeaderValue(header, type, source);
  WebVector<WebContentSecurityPolicy> policies(policies_.size() -
                                               previous_policy_count);
  for (wtf_size_t i = previous_policy_count; i < policies_.size(); ++i) {
    policies[i - previous_policy_count] =
        policies_[i]->ExposeForNavigationalChecks();
  }
  if (GetDocument() && GetDocument()->GetFrame()) {
    GetDocument()->GetFrame()->Client()->DidAddContentSecurityPolicies(
        policies);
  }
}
