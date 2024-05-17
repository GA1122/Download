bool CanonicalizePathURL(const char* spec,
                         int spec_len,
                         const Parsed& parsed,
                         CanonOutput* output,
                         Parsed* new_parsed) {
  return DoCanonicalizePathURL<char, unsigned char>(
      URLComponentSource<char>(spec), parsed, output, new_parsed);
}
