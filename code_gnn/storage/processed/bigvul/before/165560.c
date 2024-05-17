bool ContentSecurityPolicy::ShouldSendViolationReport(
    const String& report) const {
  return !violation_reports_sent_.Contains(report.Impl()->GetHash());
}
