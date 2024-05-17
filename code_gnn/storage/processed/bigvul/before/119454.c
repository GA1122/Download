ScreenOrientationDispatcherHost::ScreenOrientationDispatcherHost(
    WebContents* web_contents)
  : WebContentsObserver(web_contents),
    current_lock_(NULL) {
  provider_.reset(ScreenOrientationProvider::Create(this, web_contents));
}
