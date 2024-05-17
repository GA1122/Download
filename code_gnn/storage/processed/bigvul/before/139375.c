static bool CanReadClipboard(LocalFrame& frame, EditorCommandSource source) {
  if (source == kCommandFromMenuOrKeyBinding)
    return true;
  Settings* settings = frame.GetSettings();
  bool default_value = settings &&
                       settings->GetJavaScriptCanAccessClipboard() &&
                       settings->GetDOMPasteAllowed();
  return frame.GetEditor().Client().CanPaste(&frame, default_value);
}
