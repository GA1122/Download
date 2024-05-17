bool CanonicalizeScheme(const char* spec,
                        const Component& scheme,
                        CanonOutput* output,
                        Component* out_scheme) {
  return DoScheme<char, unsigned char>(spec, scheme, output, out_scheme);
}
