bool AXObject::hasInheritedPresentationalRole() const {
  updateCachedAttributeValuesIfNeeded();
  return m_cachedHasInheritedPresentationalRole;
}
