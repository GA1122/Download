bool AXObject::isInertOrAriaHidden() const {
  updateCachedAttributeValuesIfNeeded();
  return m_cachedIsInertOrAriaHidden;
}
