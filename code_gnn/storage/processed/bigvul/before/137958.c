bool AXLayoutObject::isOffScreen() const {
  ASSERT(m_layoutObject);
  IntRect contentRect =
      pixelSnappedIntRect(m_layoutObject->absoluteVisualRect());
  FrameView* view = m_layoutObject->frame()->view();
  IntRect viewRect = view->visibleContentRect();
  viewRect.intersect(contentRect);
  return viewRect.isEmpty();
}
