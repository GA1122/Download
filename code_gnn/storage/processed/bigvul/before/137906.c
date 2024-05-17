void AXLayoutObject::addCanvasChildren() {
  if (!isHTMLCanvasElement(getNode()))
    return;

  ASSERT(!m_children.size());
  m_haveChildren = false;
  AXNodeObject::addChildren();
}
