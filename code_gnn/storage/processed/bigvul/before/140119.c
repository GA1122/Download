void HTMLMediaElement::sourceWasRemoved(HTMLSourceElement* source) {
  BLINK_MEDIA_LOG << "sourceWasRemoved(" << (void*)this << ", " << source
                  << ")";

  KURL url = source->getNonEmptyURLAttribute(srcAttr);
  BLINK_MEDIA_LOG << "sourceWasRemoved(" << (void*)this << ") - 'src' is "
                  << urlForLoggingMedia(url);

  if (source != m_currentSourceNode && source != m_nextChildNodeToConsider)
    return;

  if (source == m_nextChildNodeToConsider) {
    if (m_currentSourceNode)
      m_nextChildNodeToConsider = m_currentSourceNode->nextSibling();
    BLINK_MEDIA_LOG << "sourceWasRemoved(" << (void*)this
                    << ") - m_nextChildNodeToConsider set to "
                    << m_nextChildNodeToConsider.get();
  } else if (source == m_currentSourceNode) {
    m_currentSourceNode = nullptr;
    BLINK_MEDIA_LOG << "sourceWasRemoved(" << (void*)this
                    << ") - m_currentSourceNode set to 0";
  }
}
