KURL HTMLMediaElement::selectNextSourceChild(ContentType* contentType,
                                             InvalidURLAction actionIfInvalid) {
  bool shouldLog = actionIfInvalid != DoNothing;
  if (shouldLog)
    BLINK_MEDIA_LOG << "selectNextSourceChild(" << (void*)this << ")";

  if (!m_nextChildNodeToConsider) {
    if (shouldLog)
      BLINK_MEDIA_LOG << "selectNextSourceChild(" << (void*)this
                      << ") -> 0x0000, \"\"";
    return KURL();
  }

  KURL mediaURL;
  Node* node;
  HTMLSourceElement* source = 0;
  String type;
  bool lookingForStartNode = m_nextChildNodeToConsider;
  bool canUseSourceElement = false;

  NodeVector potentialSourceNodes;
  getChildNodes(*this, potentialSourceNodes);

  for (unsigned i = 0; !canUseSourceElement && i < potentialSourceNodes.size();
       ++i) {
    node = potentialSourceNodes[i].get();
    if (lookingForStartNode && m_nextChildNodeToConsider != node)
      continue;
    lookingForStartNode = false;

    if (!isHTMLSourceElement(*node))
      continue;
    if (node->parentNode() != this)
      continue;

    source = toHTMLSourceElement(node);

    const AtomicString& srcValue = source->fastGetAttribute(srcAttr);
    if (shouldLog)
      BLINK_MEDIA_LOG << "selectNextSourceChild(" << (void*)this
                      << ") - 'src' is " << urlForLoggingMedia(mediaURL);
    if (srcValue.isEmpty())
      goto checkAgain;

    mediaURL = source->document().completeURL(srcValue);

    if (!isSafeToLoadURL(mediaURL, actionIfInvalid))
      goto checkAgain;

    type = source->type();
    if (type.isEmpty() && mediaURL.protocolIsData())
      type = mimeTypeFromDataURL(mediaURL);
    if (!type.isEmpty()) {
      if (shouldLog)
        BLINK_MEDIA_LOG << "selectNextSourceChild(" << (void*)this
                        << ") - 'type' is '" << type << "'";
      if (!supportsType(ContentType(type)))
        goto checkAgain;
    }

    canUseSourceElement = true;

  checkAgain:
    if (!canUseSourceElement && actionIfInvalid == Complain && source)
      source->scheduleErrorEvent();
  }

  if (canUseSourceElement) {
    if (contentType)
      *contentType = ContentType(type);
    m_currentSourceNode = source;
    m_nextChildNodeToConsider = source->nextSibling();
  } else {
    m_currentSourceNode = nullptr;
    m_nextChildNodeToConsider = nullptr;
  }

  if (shouldLog)
    BLINK_MEDIA_LOG << "selectNextSourceChild(" << (void*)this << ") -> "
                    << m_currentSourceNode.get() << ", "
                    << (canUseSourceElement ? urlForLoggingMedia(mediaURL)
                                            : "");
  return canUseSourceElement ? mediaURL : KURL();
}
