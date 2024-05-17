bool AXObject::isDescendantOfLeafNode() const {
  updateCachedAttributeValuesIfNeeded();
  return m_cachedIsDescendantOfLeafNode;
}
