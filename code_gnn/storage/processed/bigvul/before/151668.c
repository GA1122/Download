void Browser::EnterFullscreenModeForTab(WebContents* web_contents,
                                        const GURL& origin) {
  exclusive_access_manager_->fullscreen_controller()->EnterFullscreenModeForTab(
      web_contents, origin);
}
