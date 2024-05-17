void ContentSecurityPolicy::reportInvalidSandboxFlags(
    const String& invalidFlags) {
  logToConsole(
      "Error while parsing the 'sandbox' Content Security Policy directive: " +
      invalidFlags);
}
