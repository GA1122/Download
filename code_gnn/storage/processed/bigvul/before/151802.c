void BackgroundLoaderOffliner::ResetLoader() {
  loader_.reset(
      new background_loader::BackgroundLoaderContents(browser_context_));
  loader_->SetDelegate(this);
}
