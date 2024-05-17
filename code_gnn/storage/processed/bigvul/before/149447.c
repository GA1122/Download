bool isAllowedByAll(const CSPDirectiveListVector& policies,
                    Element* element,
                    const String& source,
                    const String& contextURL,
                    const WTF::OrdinalNumber& contextLine,
                    SecurityViolationReportingPolicy reportingPolicy) {
  bool isAllowed = true;
  for (const auto& policy : policies) {
    isAllowed &= (policy.get()->*allowed)(element, source, contextURL,
                                          contextLine, reportingPolicy);
  }
  return isAllowed;
}
