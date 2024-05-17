static WebFeature GetUseCounterType(ContentSecurityPolicyHeaderType type) {
  switch (type) {
    case kContentSecurityPolicyHeaderTypeEnforce:
      return WebFeature::kContentSecurityPolicy;
    case kContentSecurityPolicyHeaderTypeReport:
      return WebFeature::kContentSecurityPolicyReportOnly;
  }
  NOTREACHED();
  return WebFeature::kNumberOfFeatures;
}
