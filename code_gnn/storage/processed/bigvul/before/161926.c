bool PrintRenderFrameHelper::IsScriptInitiatedPrintAllowed(
    blink::WebLocalFrame* frame,
    bool user_initiated) {
  if (!is_printing_enabled_ || !delegate_->IsScriptedPrintEnabled())
    return false;

  return user_initiated || g_is_preview_enabled ||
         scripting_throttler_.IsAllowed(frame);
}
