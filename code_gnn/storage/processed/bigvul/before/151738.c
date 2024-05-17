bool Browser::ShouldRunUnloadListenerBeforeClosing(
    content::WebContents* web_contents) {
  if (IsFastTabUnloadEnabled())
    return fast_unload_controller_->ShouldRunUnloadEventsHelper(web_contents);
  return unload_controller_->ShouldRunUnloadEventsHelper(web_contents);
}
