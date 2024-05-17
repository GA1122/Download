void ContentSecurityPolicy::ReportDuplicateDirective(const String& name) {
  String message =
      "Ignoring duplicate Content-Security-Policy directive '" + name + "'.\n";
  LogToConsole(message);
}
