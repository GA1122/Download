static void RunAddConsoleMessageTask(MessageSource source,
                                     mojom::ConsoleMessageLevel level,
                                     const String& message,
                                     ExecutionContext* context) {
  ConsoleMessage* console_message =
      ConsoleMessage::Create(source, level, message);
  context->AddConsoleMessage(console_message);
}
