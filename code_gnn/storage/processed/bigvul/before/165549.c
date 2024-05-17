void ContentSecurityPolicy::ReportReportOnlyInMeta(const String& header) {
  LogToConsole("The report-only Content Security Policy '" + header +
               "' was delivered via a <meta> element, which is disallowed. The "
               "policy has been ignored.");
}
