void TestBackgroundLoaderOffliner::ResetLoader() {
  stub_ = new background_loader::BackgroundLoaderContentsStub(browser_context_);
  loader_.reset(stub_);
  loader_->SetDelegate(this);
}
