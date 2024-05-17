void ImageLoader::dispose() {
  RESOURCE_LOADING_DVLOG(1)
      << "~ImageLoader " << this
      << "; m_hasPendingLoadEvent=" << m_hasPendingLoadEvent
      << ", m_hasPendingErrorEvent=" << m_hasPendingErrorEvent;

  if (m_image) {
    m_image->removeObserver(this);
    m_image = nullptr;
  }
}
