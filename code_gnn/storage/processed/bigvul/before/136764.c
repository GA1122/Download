void LocalDOMWindow::PrintErrorMessage(const String& message) const {
  if (!IsCurrentlyDisplayedInFrame())
    return;

  if (message.IsEmpty())
    return;

  GetFrameConsole()->AddMessage(
      ConsoleMessage::Create(kJSMessageSource, kErrorMessageLevel, message));
}
