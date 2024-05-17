bool ContentSecurityPolicy::IsValidCSPAttr(const String& attr,
                                           const String& context_required_csp) {
  if (attr.Contains('\n') || attr.Contains('\r'))
    return false;

  ContentSecurityPolicy* attr_policy = ContentSecurityPolicy::Create();
  attr_policy->AddPolicyFromHeaderValue(attr,
                                        kContentSecurityPolicyHeaderTypeEnforce,
                                        kContentSecurityPolicyHeaderSourceHTTP);
  if (!attr_policy->console_messages_.IsEmpty() ||
      attr_policy->policies_.size() != 1) {
    return false;
  }

  for (auto& directiveList : attr_policy->policies_) {
    if (directiveList->ReportEndpoints().size() != 0)
      return false;
  }

  if (context_required_csp.IsEmpty() || context_required_csp.IsNull()) {
    return true;
  }

  ContentSecurityPolicy* context_policy = ContentSecurityPolicy::Create();
  context_policy->AddPolicyFromHeaderValue(
      context_required_csp, kContentSecurityPolicyHeaderTypeEnforce,
      kContentSecurityPolicyHeaderSourceHTTP);

  DCHECK(context_policy->console_messages_.IsEmpty() &&
         context_policy->policies_.size() == 1);

  return context_policy->Subsumes(*attr_policy);
}
