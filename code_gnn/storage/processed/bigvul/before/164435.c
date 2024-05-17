bool CanonicalizePathURL(const base::char16* spec,
                         int spec_len,
                         const Parsed& parsed,
                         CanonOutput* output,
                         Parsed* new_parsed) {
  return DoCanonicalizePathURL<base::char16, base::char16>(
      URLComponentSource<base::char16>(spec), parsed, output, new_parsed);
}
