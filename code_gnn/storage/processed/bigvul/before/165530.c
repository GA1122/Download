void ContentSecurityPolicy::LogToConsole(const String& message,
                                         MessageLevel level) {
  LogToConsole(ConsoleMessage::Create(kSecurityMessageSource, level, message));
}
