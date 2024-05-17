bool isAllowedByAll(const CSPDirectiveListVector& policies,
                    Element* element,
                    const String& contextURL,
                    const String& nonce,
                    const WTF::OrdinalNumber& contextLine,
                    SecurityViolationReportingPolicy reportingPolicy,
                    const String& content) {
  bool isAllowed = true;
  for (const auto& policy : policies) {
    isAllowed &= (policy.get()->*allowed)(
        element, contextURL, nonce, contextLine, reportingPolicy, content);
  }
  return isAllowed;
}
