void ImageLoader::dispatchPendingErrorEvent() {
  if (!m_hasPendingErrorEvent)
    return;
  m_hasPendingErrorEvent = false;

  if (element()->document().frame())
    element()->dispatchEvent(Event::create(EventTypeNames::error));

  updatedHasPendingEvent();
}
