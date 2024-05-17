void ImageLoader::dispatchPendingEvent(ImageEventSender* eventSender) {
  RESOURCE_LOADING_DVLOG(1) << "ImageLoader::dispatchPendingEvent " << this;
  DCHECK(eventSender == &loadEventSender() ||
         eventSender == &errorEventSender());
  const AtomicString& eventType = eventSender->eventType();
  if (eventType == EventTypeNames::load)
    dispatchPendingLoadEvent();
  if (eventType == EventTypeNames::error)
    dispatchPendingErrorEvent();
}
