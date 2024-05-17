AXObject* AXLayoutObject::rawNextSibling() const {
  if (!m_layoutObject)
    return 0;

  LayoutObject* nextSibling = 0;

  LayoutInline* inlineContinuation =
      m_layoutObject->isLayoutBlockFlow()
          ? toLayoutBlockFlow(m_layoutObject)->inlineElementContinuation()
          : nullptr;
  if (inlineContinuation) {
    nextSibling = firstChildConsideringContinuation(inlineContinuation);
  } else if (m_layoutObject->isAnonymousBlock() &&
             lastChildHasContinuation(m_layoutObject)) {
    LayoutObject* lastParent =
        endOfContinuations(toLayoutBlock(m_layoutObject)->lastChild())
            ->parent();
    while (lastChildHasContinuation(lastParent))
      lastParent = endOfContinuations(lastParent->slowLastChild())->parent();
    nextSibling = lastParent->nextSibling();
  } else if (LayoutObject* ns = m_layoutObject->nextSibling()) {
    nextSibling = ns;
  } else if (isInlineWithContinuation(m_layoutObject)) {
    nextSibling = endOfContinuations(m_layoutObject)->nextSibling();
  } else if (m_layoutObject->parent() &&
             isInlineWithContinuation(m_layoutObject->parent())) {
    LayoutObject* continuation =
        toLayoutInline(m_layoutObject->parent())->continuation();

    if (continuation->isLayoutBlock()) {
      nextSibling = continuation;
    } else {
      nextSibling = firstChildConsideringContinuation(continuation);
    }
  }

  if (!nextSibling)
    return 0;

  return axObjectCache().getOrCreate(nextSibling);
}
