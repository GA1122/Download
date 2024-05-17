static UseCounter::Feature getUseCounterType(
    ContentSecurityPolicyHeaderType type) {
  switch (type) {
    case ContentSecurityPolicyHeaderTypeEnforce:
      return UseCounter::ContentSecurityPolicy;
    case ContentSecurityPolicyHeaderTypeReport:
      return UseCounter::ContentSecurityPolicyReportOnly;
  }
  ASSERT_NOT_REACHED();
  return UseCounter::NumberOfFeatures;
}
