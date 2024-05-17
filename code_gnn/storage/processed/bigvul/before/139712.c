void ResourceFetcher::ClearContext() {
  ClearPreloads(ResourceFetcher::kClearAllPreloads);
  context_.Clear();
}
