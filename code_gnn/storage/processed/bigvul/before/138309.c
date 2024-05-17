DEFINE_TRACE(InspectorAccessibilityAgent) {
  visitor->trace(m_page);
  visitor->trace(m_domAgent);
  InspectorBaseAgent::trace(visitor);
}
