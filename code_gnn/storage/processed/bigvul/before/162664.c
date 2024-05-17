void CanonicalizeRef(const base::char16* spec,
                     const Component& ref,
                     CanonOutput* output,
                     Component* out_ref) {
  DoCanonicalizeRef<base::char16, base::char16>(spec, ref, output, out_ref);
}
