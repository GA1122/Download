void ImageLoader::updateFromElement(UpdateFromElementBehavior updateBehavior,
                                    ReferrerPolicy referrerPolicy) {
  AtomicString imageSourceURL = m_element->imageSourceURL();
  m_suppressErrorEvents = (updateBehavior == UpdateSizeChanged);

  if (updateBehavior == UpdateIgnorePreviousError)
    clearFailedLoadURL();

  if (!m_failedLoadURL.isEmpty() && imageSourceURL == m_failedLoadURL)
    return;

  if (m_loadingImageDocument && updateBehavior != UpdateForcedReload) {
    ImageResource* imageResource =
        ImageResource::create(imageSourceToKURL(m_element->imageSourceURL()));
    imageResource->setStatus(Resource::Pending);
    m_imageResourceForImageDocument = imageResource;
    setImage(imageResource->getContent());
    return;
  }

  if (m_pendingTask) {
    m_pendingTask->clearLoader();
    m_pendingTask.reset();
  }

  KURL url = imageSourceToKURL(imageSourceURL);
  if (shouldLoadImmediately(url)) {
    doUpdateFromElement(DoNotBypassMainWorldCSP, updateBehavior, url,
                        referrerPolicy);
    return;
  }
  if (imageSourceURL.isEmpty()) {
    ImageResourceContent* image = m_image.get();
    if (image) {
      image->removeObserver(this);
    }
    m_image = nullptr;
  }

  Document& document = m_element->document();
  if (document.isActive())
    enqueueImageLoadingMicroTask(updateBehavior, referrerPolicy);
}
