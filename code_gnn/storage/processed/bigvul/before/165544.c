void ContentSecurityPolicy::ReportInvalidSandboxFlags(
    const String& invalid_flags) {
  LogToConsole(
      "Error while parsing the 'sandbox' Content Security Policy directive: " +
      invalid_flags);
}
