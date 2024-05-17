DEFINE_TRACE(ContentSecurityPolicy) {
  visitor->trace(m_executionContext);
  visitor->trace(m_policies);
  visitor->trace(m_consoleMessages);
  visitor->trace(m_selfSource);
}
