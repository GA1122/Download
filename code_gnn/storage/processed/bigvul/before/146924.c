void WebLocalFrameImpl::AddMessageToConsole(const WebConsoleMessage& message) {
  DCHECK(GetFrame());

  MessageLevel web_core_message_level = kInfoMessageLevel;
  switch (message.level) {
    case WebConsoleMessage::kLevelVerbose:
      web_core_message_level = kVerboseMessageLevel;
      break;
    case WebConsoleMessage::kLevelInfo:
      web_core_message_level = kInfoMessageLevel;
      break;
    case WebConsoleMessage::kLevelWarning:
      web_core_message_level = kWarningMessageLevel;
      break;
    case WebConsoleMessage::kLevelError:
      web_core_message_level = kErrorMessageLevel;
      break;
  }

  GetFrame()->GetDocument()->AddConsoleMessage(ConsoleMessage::Create(
      kOtherMessageSource, web_core_message_level, message.text,
      SourceLocation::Create(message.url, message.line_number,
                             message.column_number, nullptr)));
}
