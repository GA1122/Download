bool ChromeClientImpl::ShouldOpenModalDialogDuringPageDismissal(
    LocalFrame& frame,
    DialogType dialog_type,
    const String& dialog_message,
    Document::PageDismissalType dismissal_type) const {
  String message = String("Blocked ") + DialogTypeToString(dialog_type) + "('" +
                   dialog_message + "') during " +
                   DismissalTypeToString(dismissal_type) + ".";
  WebLocalFrameImpl::FromFrame(frame)->AddMessageToConsole(
      WebConsoleMessage(WebConsoleMessage::kLevelError, message));

  return false;
}
