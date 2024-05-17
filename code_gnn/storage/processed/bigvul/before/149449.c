bool isAllowedByAll(const CSPDirectiveListVector& policies,
                    const String& contextURL,
                    const String& nonce,
                    ParserDisposition parserDisposition,
                    const WTF::OrdinalNumber& contextLine,
                    SecurityViolationReportingPolicy reportingPolicy,
                    const String& content) {
  bool isAllowed = true;
  for (const auto& policy : policies) {
    isAllowed &=
        (policy.get()->*allowed)(contextURL, nonce, parserDisposition,
                                 contextLine, reportingPolicy, content);
  }
  return isAllowed;
}
