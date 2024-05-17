DEFINE_TRACE(AXObject) {
  visitor->trace(m_children);
  visitor->trace(m_parent);
  visitor->trace(m_cachedLiveRegionRoot);
  visitor->trace(m_axObjectCache);
}
