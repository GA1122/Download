bool ContentSecurityPolicy::shouldSendViolationReport(
    const String& report) const {
  return !m_violationReportsSent.contains(report.impl()->hash());
}
