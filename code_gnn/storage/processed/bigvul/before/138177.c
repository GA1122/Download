bool AXObject::isPresentationalChild() const {
  updateCachedAttributeValuesIfNeeded();
  return m_cachedIsPresentationalChild;
}
