bool ContentSecurityPolicy::ShouldBypassContentSecurityPolicy(
    const KURL& url,
    ExecutionContext* execution_context,
    SchemeRegistry::PolicyAreas area) {
  bool should_bypass_csp;
  if (SecurityOrigin::ShouldUseInnerURL(url)) {
    should_bypass_csp = SchemeRegistry::SchemeShouldBypassContentSecurityPolicy(
        SecurityOrigin::ExtractInnerURL(url).Protocol(), area);
    if (should_bypass_csp) {
      UseCounter::Count(execution_context, WebFeature::kInnerSchemeBypassesCSP);
    }
  } else {
    should_bypass_csp = SchemeRegistry::SchemeShouldBypassContentSecurityPolicy(
        url.Protocol(), area);
  }
  if (should_bypass_csp) {
    UseCounter::Count(execution_context, WebFeature::kSchemeBypassesCSP);
  }

  return should_bypass_csp;
}
