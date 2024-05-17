bool ContentSecurityPolicy::allowScriptFromSource(
    const KURL& url,
    const String& nonce,
    ParserDisposition parserDisposition,
    RedirectStatus redirectStatus,
    SecurityViolationReportingPolicy reportingPolicy) const {
  if (shouldBypassContentSecurityPolicy(url)) {
    UseCounter::count(
        document(),
        parserDisposition == ParserInserted
            ? UseCounter::ScriptWithCSPBypassingSchemeParserInserted
            : UseCounter::ScriptWithCSPBypassingSchemeNotParserInserted);
  }
  return isAllowedByAll<&CSPDirectiveList::allowScriptFromSource>(
      m_policies, url, nonce, parserDisposition, redirectStatus,
      reportingPolicy);
}
