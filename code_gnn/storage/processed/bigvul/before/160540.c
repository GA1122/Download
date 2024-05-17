void RenderFrameImpl::AddMessageToConsole(ConsoleMessageLevel level,
                                          const std::string& message) {
  blink::WebConsoleMessage::Level target_level =
      blink::WebConsoleMessage::kLevelInfo;
  switch (level) {
    case CONSOLE_MESSAGE_LEVEL_VERBOSE:
      target_level = blink::WebConsoleMessage::kLevelVerbose;
      break;
    case CONSOLE_MESSAGE_LEVEL_INFO:
      target_level = blink::WebConsoleMessage::kLevelInfo;
      break;
    case CONSOLE_MESSAGE_LEVEL_WARNING:
      target_level = blink::WebConsoleMessage::kLevelWarning;
      break;
    case CONSOLE_MESSAGE_LEVEL_ERROR:
      target_level = blink::WebConsoleMessage::kLevelError;
      break;
  }

  blink::WebConsoleMessage wcm(target_level, WebString::FromUTF8(message));
  frame_->AddMessageToConsole(wcm);
}
