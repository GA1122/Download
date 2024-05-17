void ContentSecurityPolicy::reportInvalidDirectiveInMeta(
    const String& directive) {
  logToConsole(
      "Content Security Policies delivered via a <meta> element may not "
      "contain the " +
      directive + " directive.");
}
