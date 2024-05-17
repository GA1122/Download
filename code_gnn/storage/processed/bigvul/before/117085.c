bool SessionService::RestoreIfNecessary(const std::vector<GURL>& urls_to_open) {
  return RestoreIfNecessary(urls_to_open, NULL);
}
