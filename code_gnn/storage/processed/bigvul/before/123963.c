void RenderViewImpl::didAddMessageToConsole(
    const WebConsoleMessage& message, const WebString& source_name,
    unsigned source_line) {
  logging::LogSeverity log_severity = logging::LOG_VERBOSE;
  switch (message.level) {
    case WebConsoleMessage::LevelDebug:
      log_severity = logging::LOG_VERBOSE;
      break;
    case WebConsoleMessage::LevelLog:
      log_severity = logging::LOG_INFO;
      break;
    case WebConsoleMessage::LevelWarning:
      log_severity = logging::LOG_WARNING;
      break;
    case WebConsoleMessage::LevelError:
      log_severity = logging::LOG_ERROR;
      break;
    default:
      NOTREACHED();
  }

  Send(new ViewHostMsg_AddMessageToConsole(routing_id_,
                                           static_cast<int32>(log_severity),
                                           message.text,
                                           static_cast<int32>(source_line),
                                           source_name));
}
