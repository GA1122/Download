void Document::WillInsertBody() {
  if (Loader())
    fetcher_->LoosenLoadThrottlingPolicy();

  BeginLifecycleUpdatesIfRenderingReady();
}
