void WebGLRenderingContextBase::PrintWarningToConsole(const String& message) {
  if (!canvas())
    return;
  canvas()->GetDocument().AddConsoleMessage(ConsoleMessage::Create(
      kRenderingMessageSource, kWarningMessageLevel, message));
}
