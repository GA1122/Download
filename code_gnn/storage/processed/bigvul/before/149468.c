void ContentSecurityPolicy::reportInvalidRequireSRIForTokens(
    const String& invalidTokens) {
  logToConsole(
      "Error while parsing the 'require-sri-for' Content Security Policy "
      "directive: " +
      invalidTokens);
}
