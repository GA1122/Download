RGBA32 AXObject::backgroundColor() const {
  updateCachedAttributeValuesIfNeeded();
  return m_cachedBackgroundColor;
}
