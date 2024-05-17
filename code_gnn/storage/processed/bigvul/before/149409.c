bool ContentSecurityPolicy::allowInlineScript(
    Element* element,
    const String& contextURL,
    const String& nonce,
    const WTF::OrdinalNumber& contextLine,
    const String& scriptContent,
    SecurityViolationReportingPolicy reportingPolicy) const {
  DCHECK(element);
  return isAllowedByAll<&CSPDirectiveList::allowInlineScript>(
      m_policies, element, contextURL, nonce, contextLine, reportingPolicy,
      scriptContent);
}
