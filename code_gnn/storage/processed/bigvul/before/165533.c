bool ContentSecurityPolicy::ProtocolEqualsSelf(const String& protocol) const {
  return EqualIgnoringASCIICase(protocol, self_protocol_);
}
