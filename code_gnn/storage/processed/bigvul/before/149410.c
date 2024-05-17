bool ContentSecurityPolicy::allowInlineStyle(
    Element* element,
    const String& contextURL,
    const String& nonce,
    const WTF::OrdinalNumber& contextLine,
    const String& styleContent,
    SecurityViolationReportingPolicy reportingPolicy) const {
  DCHECK(element);
  if (m_overrideInlineStyleAllowed)
    return true;
  return isAllowedByAll<&CSPDirectiveList::allowInlineStyle>(
      m_policies, element, contextURL, nonce, contextLine, reportingPolicy,
      styleContent);
}
