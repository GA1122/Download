ScrollableArea* AXLayoutObject::getScrollableAreaIfScrollable() const {
  if (isWebArea())
    return documentFrameView()->layoutViewportScrollableArea();

  if (!m_layoutObject || !m_layoutObject->isBox())
    return 0;

  LayoutBox* box = toLayoutBox(m_layoutObject);
  if (!box->canBeScrolledAndHasScrollableArea())
    return 0;

  return box->getScrollableArea();
}
