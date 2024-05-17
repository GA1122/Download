inline void ImageLoader::crossSiteOrCSPViolationOccurred(
    AtomicString imageSourceURL) {
  m_failedLoadURL = imageSourceURL;
}
