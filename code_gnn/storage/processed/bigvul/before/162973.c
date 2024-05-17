ManifestManager::~ManifestManager() {
  if (fetcher_)
    fetcher_->Cancel();

  ResolveCallbacks(ResolveStateFailure);
}
