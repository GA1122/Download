bool CaptivePortalDetector::FetchingURL() const {
  return url_fetcher_.get() != NULL;
}
