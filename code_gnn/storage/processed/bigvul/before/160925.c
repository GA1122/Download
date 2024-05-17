void ChromeClientImpl::AddMessageToConsole(LocalFrame* local_frame,
                                           MessageSource source,
                                           MessageLevel level,
                                           const String& message,
                                           unsigned line_number,
                                           const String& source_id,
                                           const String& stack_trace) {
  WebLocalFrameImpl* frame = WebLocalFrameImpl::FromFrame(local_frame);
  if (frame && frame->Client()) {
    frame->Client()->DidAddMessageToConsole(
        WebConsoleMessage(static_cast<WebConsoleMessage::Level>(level),
                          message),
        source_id, line_number, stack_trace);
  }
}
