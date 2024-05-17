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

  MessageSource message_source = message.nodes.empty()
                                     ? kOtherMessageSource
                                     : kRecommendationMessageSource;
  Vector<DOMNodeId> nodes;
  for (const blink::WebNode& web_node : message.nodes)
    nodes.push_back(DOMNodeIds::IdForNode(&(*web_node)));
  ConsoleMessage* console_message = ConsoleMessage::Create(
      message_source, web_core_message_level, message.text,
      SourceLocation::Create(message.url, message.line_number,
                             message.column_number, nullptr));
  console_message->SetNodes(GetFrame(), std::move(nodes));
  GetFrame()->GetDocument()->AddConsoleMessage(console_message);
}
