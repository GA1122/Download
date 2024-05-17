void Document::WillInsertBody() {
  if (auto* loader = Loader())
    loader->Fetcher()->LoosenLoadThrottlingPolicy();

  BeginLifecycleUpdatesIfRenderingReady();
}
