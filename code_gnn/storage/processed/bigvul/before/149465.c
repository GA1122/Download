void ContentSecurityPolicy::reportInvalidInReportOnly(const String& name) {
  logToConsole("The Content Security Policy directive '" + name +
               "' is ignored when delivered in a report-only policy.");
}
