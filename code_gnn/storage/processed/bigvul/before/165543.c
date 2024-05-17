void ContentSecurityPolicy::ReportInvalidRequireSRIForTokens(
    const String& invalid_tokens) {
  LogToConsole(
      "Error while parsing the 'require-sri-for' Content Security Policy "
      "directive: " +
      invalid_tokens);
}
