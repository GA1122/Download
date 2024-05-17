void ContentSecurityPolicy::FillInCSPHashValues(
    const String& source,
    uint8_t hash_algorithms_used,
    Vector<CSPHashValue>* csp_hash_values) {
  static const struct {
    ContentSecurityPolicyHashAlgorithm csp_hash_algorithm;
    HashAlgorithm algorithm;
  } kAlgorithmMap[] = {
      {kContentSecurityPolicyHashAlgorithmSha256, kHashAlgorithmSha256},
      {kContentSecurityPolicyHashAlgorithmSha384, kHashAlgorithmSha384},
      {kContentSecurityPolicyHashAlgorithmSha512, kHashAlgorithmSha512}};

  if (hash_algorithms_used == kContentSecurityPolicyHashAlgorithmNone)
    return;

  StringUTF8Adaptor utf8_source(
      source, kStrictUTF8ConversionReplacingUnpairedSurrogatesWithFFFD);

  for (const auto& algorithm_map : kAlgorithmMap) {
    DigestValue digest;
    if (algorithm_map.csp_hash_algorithm & hash_algorithms_used) {
      bool digest_success =
          ComputeDigest(algorithm_map.algorithm, utf8_source.Data(),
                        utf8_source.length(), digest);
      if (digest_success) {
        csp_hash_values->push_back(
            CSPHashValue(algorithm_map.csp_hash_algorithm, digest));
      }
    }
  }
}
