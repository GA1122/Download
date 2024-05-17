void BackgroundLoaderOffliner::AttachObservers() {
  content::WebContents* contents = loader_->web_contents();
  content::WebContentsObserver::Observe(contents);
  OfflinerUserData::AddToWebContents(contents, this);
}
