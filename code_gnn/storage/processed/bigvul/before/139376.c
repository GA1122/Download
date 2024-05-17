static bool CanWriteClipboard(LocalFrame& frame, EditorCommandSource source) {
  if (source == kCommandFromMenuOrKeyBinding)
    return true;
  Settings* settings = frame.GetSettings();
  bool default_value =
      (settings && settings->GetJavaScriptCanAccessClipboard()) ||
      UserGestureIndicator::ProcessingUserGesture();
  return frame.GetEditor().Client().CanCopyCut(&frame, default_value);
}
