bool ContentSecurityPolicy::allowInlineEventHandler(
    Element* element,
    const String& source,
    const String& contextURL,
    const WTF::OrdinalNumber& contextLine,
    SecurityViolationReportingPolicy reportingPolicy) const {
  if (checkDigest<&CSPDirectiveList::allowScriptHash>(
          source, InlineType::Attribute, m_scriptHashAlgorithmsUsed,
          m_policies))
    return true;
  return isAllowedByAll<&CSPDirectiveList::allowInlineEventHandlers>(
      m_policies, element, source, contextURL, contextLine, reportingPolicy);
}
