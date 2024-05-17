bool HTMLMediaElement::hasPendingActivity() const {
  if (m_shouldDelayLoadEvent)
    return true;

  if (m_networkState == kNetworkLoading)
    return true;

  {
    AutoReset<bool> scope(&m_officialPlaybackPositionNeedsUpdate, false);

    if (couldPlayIfEnoughData())
      return true;
  }

  if (m_seeking)
    return true;

  if (m_mediaSource)
    return true;

  if (m_asyncEventQueue->hasPendingEvents())
    return true;

  return false;
}
