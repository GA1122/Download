void ContentSecurityPolicy::usesScriptHashAlgorithms(uint8_t algorithms) {
  m_scriptHashAlgorithmsUsed |= algorithms;
}
