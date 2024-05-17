bool AXObject::accessibilityIsIgnored() const {
  updateCachedAttributeValuesIfNeeded();
  return m_cachedIsIgnored;
}
