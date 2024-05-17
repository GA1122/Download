void CanonicalizeRef(const char* spec,
                     const Component& ref,
                     CanonOutput* output,
                     Component* out_ref) {
  DoCanonicalizeRef<char, unsigned char>(spec, ref, output, out_ref);
}
