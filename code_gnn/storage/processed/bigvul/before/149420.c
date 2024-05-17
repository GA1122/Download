bool ContentSecurityPolicy::allowScriptWithHash(const String& source,
                                                InlineType type) const {
  return checkDigest<&CSPDirectiveList::allowScriptHash>(
      source, type, m_scriptHashAlgorithmsUsed, m_policies);
}
