void ContentSecurityPolicy::ReportMetaOutsideHead(const String& header) {
  LogToConsole("The Content Security Policy '" + header +
               "' was delivered via a <meta> element outside the document's "
               "<head>, which is disallowed. The policy has been ignored.");
}
