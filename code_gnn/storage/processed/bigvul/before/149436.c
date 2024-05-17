void ContentSecurityPolicy::enforceStrictMixedContentChecking() {
  m_insecureRequestPolicy |= kBlockAllMixedContent;
}
