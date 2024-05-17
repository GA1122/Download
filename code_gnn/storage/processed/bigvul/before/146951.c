void WebLocalFrameImpl::DeleteSurroundingText(int before, int after) {
  TRACE_EVENT0("blink", "WebLocalFrameImpl::deleteSurroundingText");
  if (WebPlugin* plugin = FocusedPluginIfInputMethodSupported()) {
    plugin->DeleteSurroundingText(before, after);
    return;
  }

  GetFrame()->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  GetFrame()->GetInputMethodController().DeleteSurroundingText(before, after);
}
