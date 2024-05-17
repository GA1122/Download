void ContentSecurityPolicy::reportMetaOutsideHead(const String& header) {
  logToConsole("The Content Security Policy '" + header +
               "' was delivered via a <meta> element outside the document's "
               "<head>, which is disallowed. The policy has been ignored.");
}
