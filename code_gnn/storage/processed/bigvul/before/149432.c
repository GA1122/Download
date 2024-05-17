void ContentSecurityPolicy::didSendViolationReport(const String& report) {
  m_violationReportsSent.insert(report.impl()->hash());
}
