int ResourceFetcher::NonblockingRequestCount() const {
  return non_blocking_loaders_.size();
}
