void LocalFrame::PrintNavigationWarning(const String& message) {
  console_->AddMessage(
      ConsoleMessage::Create(kJSMessageSource, kWarningMessageLevel, message));
}
