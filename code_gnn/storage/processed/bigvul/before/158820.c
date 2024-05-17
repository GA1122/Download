void Browser::EnterFullscreenModeForTab(
    WebContents* web_contents,
    const GURL& origin,
    const blink::WebFullscreenOptions& options) {
  exclusive_access_manager_->fullscreen_controller()->EnterFullscreenModeForTab(
      web_contents, origin);
}
