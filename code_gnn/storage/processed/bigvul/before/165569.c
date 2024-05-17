void ContentSecurityPolicy::UsesStyleHashAlgorithms(uint8_t algorithms) {
  style_hash_algorithms_used_ |= algorithms;
}
