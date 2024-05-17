bool PrintWebViewHelper::IsScriptInitiatedPrintAllowed(
    WebKit::WebFrame* frame) {
  if (is_scripted_printing_blocked_)
    return false;
  return !IsScriptInitiatedPrintTooFrequent(frame);
}
