void MetricsWebContentsObserver::WebContentsWillSoonBeDestroyed() {
  web_contents_will_soon_be_destroyed_ = true;
}
