void Browser::ExitFullscreenModeForTab(WebContents* web_contents) {
  exclusive_access_manager_->fullscreen_controller()->ExitFullscreenModeForTab(
      web_contents);
}
