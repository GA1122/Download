bool isAllowedByAll(const CSPDirectiveListVector& policies,
                    const KURL& url,
                    const String& nonce,
                    ParserDisposition parserDisposition,
                    RedirectStatus redirectStatus,
                    SecurityViolationReportingPolicy reportingPolicy) {
  if (ContentSecurityPolicy::shouldBypassContentSecurityPolicy(url)) {
    if (parserDisposition == NotParserInserted ||
        !RuntimeEnabledFeatures::
            experimentalContentSecurityPolicyFeaturesEnabled()) {
      return true;
    }
  }

  bool isAllowed = true;
  for (const auto& policy : policies) {
    isAllowed &= (policy.get()->*allowFromURLWithNonceAndParser)(
        url, nonce, parserDisposition, redirectStatus, reportingPolicy);
  }
  return isAllowed;
}
