void ContentSecurityPolicy::UsesScriptHashAlgorithms(uint8_t algorithms) {
  script_hash_algorithms_used_ |= algorithms;
}
