 void MediaElementAudioSourceHandler::PrintCORSMessage(const String& message) {
  if (Context()->GetExecutionContext()) {
    Context()->GetExecutionContext()->AddConsoleMessage(
        ConsoleMessage::Create(kSecurityMessageSource, kInfoMessageLevel,
                               "MediaElementAudioSource outputs zeroes due to "
                               "CORS access restrictions for " +
                                   message));
  }
}
