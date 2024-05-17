bool PrintWebViewHelper::IsScriptInitiatedPrintAllowed(
    WebKit::WebFrame* frame, bool user_initiated) {
  if (is_scripted_printing_blocked_)
    return false;
  if (!is_scripted_print_throttling_disabled_ &&
      !is_preview_enabled_ &&
      !user_initiated)
    return !IsScriptInitiatedPrintTooFrequent(frame);
  return true;
}
