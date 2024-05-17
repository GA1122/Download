void ContentSecurityPolicy::reportMissingReportURI(const String& policy) {
  logToConsole("The Content Security Policy '" + policy +
               "' was delivered in report-only mode, but does not specify a "
               "'report-uri'; the policy will have no effect. Please either "
               "add a 'report-uri' directive, or deliver the policy via the "
               "'Content-Security-Policy' header.");
}
