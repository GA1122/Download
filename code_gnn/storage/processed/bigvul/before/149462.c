void ContentSecurityPolicy::reportDuplicateDirective(const String& name) {
  String message =
      "Ignoring duplicate Content-Security-Policy directive '" + name + "'.\n";
  logToConsole(message);
}
