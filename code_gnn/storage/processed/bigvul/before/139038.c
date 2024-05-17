void ImageLoader::updatedHasPendingEvent() {
  bool wasProtected = m_elementIsProtected;
  m_elementIsProtected = m_hasPendingLoadEvent || m_hasPendingErrorEvent;
  if (wasProtected == m_elementIsProtected)
    return;

  if (m_elementIsProtected) {
    if (m_derefElementTimer.isActive())
      m_derefElementTimer.stop();
    else
      m_keepAlive = m_element;
  } else {
    DCHECK(!m_derefElementTimer.isActive());
    m_derefElementTimer.startOneShot(0, BLINK_FROM_HERE);
  }
}
