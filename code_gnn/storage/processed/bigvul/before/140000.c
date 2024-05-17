bool HTMLMediaElement::havePotentialSourceChild() {
  HTMLSourceElement* currentSourceNode = m_currentSourceNode;
  Node* nextNode = m_nextChildNodeToConsider;

  KURL nextURL = selectNextSourceChild(0, DoNothing);

  m_currentSourceNode = currentSourceNode;
  m_nextChildNodeToConsider = nextNode;

  return nextURL.isValid();
}
