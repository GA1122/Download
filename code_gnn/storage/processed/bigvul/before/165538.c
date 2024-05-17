void ContentSecurityPolicy::ReportInvalidDirectiveInMeta(
    const String& directive) {
  LogToConsole(
      "Content Security Policies delivered via a <meta> element may not "
      "contain the " +
      directive + " directive.");
}
