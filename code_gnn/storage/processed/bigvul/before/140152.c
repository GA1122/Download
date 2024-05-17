void CaptivePortalDetector::Cancel() {
  url_fetcher_.reset();
  detection_callback_.Reset();
}
