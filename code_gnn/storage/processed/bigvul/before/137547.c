bool PrintWebViewHelper::IsScriptInitiatedPrintAllowed(blink::WebFrame* frame,
                                                       bool user_initiated) {
  if (!delegate_->IsScriptedPrintEnabled())
    return false;

  return !is_scripted_printing_blocked_ &&
         (user_initiated || g_is_preview_enabled_ ||
          scripting_throttler_.IsAllowed(frame));
}
