static void RunAddConsoleMessageTask(MessageSource source,
                                     MessageLevel level,
                                     const String& message,
                                     ExecutionContext* context) {
  context->AddConsoleMessage(ConsoleMessage::Create(source, level, message));
}
