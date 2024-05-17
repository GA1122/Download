bool Browser::RunUnloadListenerBeforeClosing(
    content::WebContents* web_contents) {
  if (IsFastTabUnloadEnabled())
    return fast_unload_controller_->RunUnloadEventsHelper(web_contents);
  return unload_controller_->RunUnloadEventsHelper(web_contents);
}
