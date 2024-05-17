LayoutBoxModelObject* AXLayoutObject::getLayoutBoxModelObject() const {
  if (!m_layoutObject || !m_layoutObject->isBoxModelObject())
    return 0;
  return toLayoutBoxModelObject(m_layoutObject);
}
