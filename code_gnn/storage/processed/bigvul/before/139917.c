DEFINE_TRACE(HTMLMediaElement) {
  visitor->trace(m_playedTimeRanges);
  visitor->trace(m_asyncEventQueue);
  visitor->trace(m_error);
  visitor->trace(m_currentSourceNode);
  visitor->trace(m_nextChildNodeToConsider);
  visitor->trace(m_mediaSource);
  visitor->trace(m_audioTracks);
  visitor->trace(m_videoTracks);
  visitor->trace(m_cueTimeline);
  visitor->trace(m_textTracks);
  visitor->trace(m_textTracksWhenResourceSelectionBegan);
  visitor->trace(m_playPromiseResolvers);
  visitor->trace(m_playPromiseResolveList);
  visitor->trace(m_playPromiseRejectList);
  visitor->trace(m_audioSourceProvider);
  visitor->trace(m_autoplayUmaHelper);
  visitor->trace(m_srcObject);
  visitor->trace(m_autoplayVisibilityObserver);
  visitor->trace(m_mediaControls);
  visitor->template registerWeakMembers<HTMLMediaElement,
                                        &HTMLMediaElement::clearWeakMembers>(
      this);
  Supplementable<HTMLMediaElement>::trace(visitor);
  HTMLElement::trace(visitor);
  SuspendableObject::trace(visitor);
}
