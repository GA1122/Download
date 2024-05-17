int ResourceFetcher::BlockingRequestCount() const {
  return loaders_.size();
}
