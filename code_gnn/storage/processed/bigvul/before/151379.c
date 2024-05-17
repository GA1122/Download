void BaseFetchContext::AddWarningConsoleMessage(const String& message,
                                                LogSource source) const {
  DCHECK_EQ(source, kJSSource);
  AddConsoleMessage(
      ConsoleMessage::Create(kJSMessageSource, kWarningMessageLevel, message));
}
