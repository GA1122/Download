bool AXObject::ancestorExposesActiveDescendant() const {
  updateCachedAttributeValuesIfNeeded();
  return m_cachedAncestorExposesActiveDescendant;
}
