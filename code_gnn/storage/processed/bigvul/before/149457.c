void ContentSecurityPolicy::logToConsole(const String& message,
                                         MessageLevel level) {
  logToConsole(ConsoleMessage::create(SecurityMessageSource, level, message));
}
