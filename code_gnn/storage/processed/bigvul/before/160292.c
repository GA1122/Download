inline void ImageLoader::CrossSiteOrCSPViolationOccurred(
    AtomicString image_source_url) {
  failed_load_url_ = image_source_url;
}
