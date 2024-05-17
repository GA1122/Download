bool ContentSecurityPolicy::allowWorkerContextFromSource(
    const KURL& url,
    RedirectStatus redirectStatus,
    SecurityViolationReportingPolicy reportingPolicy) const {
  if (Document* document = this->document()) {
    UseCounter::count(*document, UseCounter::WorkerSubjectToCSP);
    if (isAllowedByAll<&CSPDirectiveList::allowWorkerFromSource>(
            m_policies, url, redirectStatus,
            SecurityViolationReportingPolicy::SuppressReporting) &&
        !isAllowedByAll<&CSPDirectiveList::allowScriptFromSource>(
            m_policies, url, AtomicString(), NotParserInserted, redirectStatus,
            SecurityViolationReportingPolicy::SuppressReporting)) {
      UseCounter::count(*document,
                        UseCounter::WorkerAllowedByChildBlockedByScript);
    }
  }

  return isAllowedByAll<&CSPDirectiveList::allowWorkerFromSource>(
      m_policies, url, redirectStatus, reportingPolicy);
}
