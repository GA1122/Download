void ContentSecurityPolicy::reportReportOnlyInMeta(const String& header) {
  logToConsole("The report-only Content Security Policy '" + header +
               "' was delivered via a <meta> element, which is disallowed. The "
               "policy has been ignored.");
}
