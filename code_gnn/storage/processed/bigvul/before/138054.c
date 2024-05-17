void AXNodeObject::init() {
#if DCHECK_IS_ON()
  ASSERT(!m_initialized);
  m_initialized = true;
#endif
  m_role = determineAccessibilityRole();
}
