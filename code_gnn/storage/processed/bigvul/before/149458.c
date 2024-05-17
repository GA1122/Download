bool ContentSecurityPolicy::protocolEqualsSelf(const String& protocol) const {
  return equalIgnoringCase(protocol, m_selfProtocol);
}
