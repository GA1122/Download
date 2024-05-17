bool ContentSecurityPolicy::allowStyleWithHash(const String& source,
                                               InlineType type) const {
  return checkDigest<&CSPDirectiveList::allowStyleHash>(
      source, type, m_styleHashAlgorithmsUsed, m_policies);
}
