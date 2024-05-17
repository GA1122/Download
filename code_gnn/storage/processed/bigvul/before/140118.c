void HTMLMediaElement::sourceWasAdded(HTMLSourceElement* source) {
  BLINK_MEDIA_LOG << "sourceWasAdded(" << (void*)this << ", " << source << ")";

  KURL url = source->getNonEmptyURLAttribute(srcAttr);
  BLINK_MEDIA_LOG << "sourceWasAdded(" << (void*)this << ") - 'src' is "
                  << urlForLoggingMedia(url);

  if (fastHasAttribute(srcAttr))
    return;

  if (getNetworkState() == HTMLMediaElement::kNetworkEmpty) {
    invokeResourceSelectionAlgorithm();
    m_nextChildNodeToConsider = source;
    return;
  }

  if (m_currentSourceNode && source == m_currentSourceNode->nextSibling()) {
    BLINK_MEDIA_LOG << "sourceWasAdded(" << (void*)this
                    << ") - <source> inserted immediately after current source";
    m_nextChildNodeToConsider = source;
    return;
  }

  if (m_nextChildNodeToConsider)
    return;

  if (m_loadState != WaitingForSource)
    return;

  setShouldDelayLoadEvent(true);

  setNetworkState(kNetworkLoading);

  m_nextChildNodeToConsider = source;
  scheduleNextSourceChild();
}
