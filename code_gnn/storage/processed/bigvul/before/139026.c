void ImageLoader::imageNotifyFinished(ImageResourceContent* resource) {
  RESOURCE_LOADING_DVLOG(1)
      << "ImageLoader::imageNotifyFinished " << this
      << "; m_hasPendingLoadEvent=" << m_hasPendingLoadEvent;

  DCHECK(m_failedLoadURL.isEmpty());
  DCHECK_EQ(resource, m_image.get());

  m_imageComplete = true;

  if (m_image)
    m_image->updateImageAnimationPolicy();

  updateLayoutObject();

  if (m_image && m_image->getImage() && m_image->getImage()->isSVGImage())
    toSVGImage(m_image->getImage())->updateUseCounters(element()->document());

  if (!m_hasPendingLoadEvent)
    return;

  if (resource->errorOccurred()) {
    loadEventSender().cancelEvent(this);
    m_hasPendingLoadEvent = false;

    if (resource->resourceError().isAccessCheck()) {
      crossSiteOrCSPViolationOccurred(
          AtomicString(resource->resourceError().failingURL()));
    }

    if (!m_suppressErrorEvents)
      dispatchErrorEvent();

    updatedHasPendingEvent();
    return;
  }
  loadEventSender().dispatchEventSoon(this);
}
