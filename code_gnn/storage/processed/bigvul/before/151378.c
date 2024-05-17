void BaseFetchContext::AddErrorConsoleMessage(const String& message,
                                              LogSource source) const {
  DCHECK_EQ(source, kJSSource);
  AddConsoleMessage(
      ConsoleMessage::Create(kJSMessageSource, kErrorMessageLevel, message));
}
