bool Browser::IsFullscreenForTabOrPending(
    const WebContents* web_contents) const {
  return exclusive_access_manager_->fullscreen_controller()
      ->IsFullscreenForTabOrPending(web_contents);
}
