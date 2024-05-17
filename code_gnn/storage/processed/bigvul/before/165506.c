void ContentSecurityPolicy::CopyStateFrom(const ContentSecurityPolicy* other) {
  DCHECK(policies_.IsEmpty());
  for (const auto& policy : other->policies_)
    AddAndReportPolicyFromHeaderValue(policy->Header(), policy->HeaderType(),
                                      policy->HeaderSource());
}
