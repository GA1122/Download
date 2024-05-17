void InputType::AddWarningToConsole(const char* message_format,
                                    const String& value) const {
  GetElement().GetDocument().AddConsoleMessage(ConsoleMessage::Create(
      kRenderingMessageSource, kWarningMessageLevel,
      String::Format(message_format,
                     JSONValue::QuoteString(value).Utf8().data())));
}
