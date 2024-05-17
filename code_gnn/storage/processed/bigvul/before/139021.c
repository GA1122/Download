void ImageLoader::doUpdateFromElement(BypassMainWorldBehavior bypassBehavior,
                                      UpdateFromElementBehavior updateBehavior,
                                      const KURL& url,
                                      ReferrerPolicy referrerPolicy) {
  m_pendingTask.reset();
  std::unique_ptr<IncrementLoadEventDelayCount> loadDelayCounter;
  loadDelayCounter.swap(m_loadDelayCounter);

  Document& document = m_element->document();
  if (!document.isActive())
    return;

  AtomicString imageSourceURL = m_element->imageSourceURL();
  ImageResourceContent* newImage = nullptr;
  if (!url.isNull()) {
    ResourceLoaderOptions resourceLoaderOptions =
        ResourceFetcher::defaultResourceOptions();
    ResourceRequest resourceRequest(url);
    if (updateBehavior == UpdateForcedReload) {
      resourceRequest.setCachePolicy(WebCachePolicy::BypassingCache);
      resourceRequest.setPreviewsState(WebURLRequest::PreviewsNoTransform);
    }

    if (referrerPolicy != ReferrerPolicyDefault) {
      resourceRequest.setHTTPReferrer(SecurityPolicy::generateReferrer(
          referrerPolicy, url, document.outgoingReferrer()));
    }

    if (isHTMLPictureElement(element()->parentNode()) ||
        !element()->fastGetAttribute(HTMLNames::srcsetAttr).isNull())
      resourceRequest.setRequestContext(WebURLRequest::RequestContextImageSet);
    FetchRequest request(resourceRequest, element()->localName(),
                         resourceLoaderOptions);
    configureRequest(request, bypassBehavior, *m_element,
                     document.clientHintsPreferences());

    if (updateBehavior != UpdateForcedReload && document.settings() &&
        document.settings()->getFetchImagePlaceholders()) {
      request.setAllowImagePlaceholder();
    }

    newImage = ImageResourceContent::fetch(request, document.fetcher());

    if (!newImage && !pageIsBeingDismissed(&document)) {
      crossSiteOrCSPViolationOccurred(imageSourceURL);
      dispatchErrorEvent();
    } else {
      clearFailedLoadURL();
    }
  } else {
    if (!imageSourceURL.isNull()) {
      dispatchErrorEvent();
    }
    noImageResourceToLoad();
  }

  ImageResourceContent* oldImage = m_image.get();
  if (updateBehavior == UpdateSizeChanged && m_element->layoutObject() &&
      m_element->layoutObject()->isImage() && newImage == oldImage) {
    toLayoutImage(m_element->layoutObject())->intrinsicSizeChanged();
  } else {
    if (m_hasPendingLoadEvent) {
      loadEventSender().cancelEvent(this);
      m_hasPendingLoadEvent = false;
    }

    if (m_hasPendingErrorEvent && newImage) {
      errorEventSender().cancelEvent(this);
      m_hasPendingErrorEvent = false;
    }

    m_image = newImage;
    m_hasPendingLoadEvent = newImage;
    m_imageComplete = !newImage;

    updateLayoutObject();
    if (newImage) {
      newImage->addObserver(this);
    }
    if (oldImage) {
      oldImage->removeObserver(this);
    }
  }

  if (LayoutImageResource* imageResource = layoutImageResource())
    imageResource->resetAnimation();

  updatedHasPendingEvent();
}
