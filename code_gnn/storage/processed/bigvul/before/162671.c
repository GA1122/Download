bool DoScheme(const CHAR* spec,
              const Component& scheme,
              CanonOutput* output,
              Component* out_scheme) {
  if (scheme.len <= 0) {
    *out_scheme = Component(output->length(), 0);
    output->push_back(':');
    return false;
  }

  out_scheme->begin = output->length();

  bool success = true;
  int end = scheme.end();
  for (int i = scheme.begin; i < end; i++) {
    UCHAR ch = static_cast<UCHAR>(spec[i]);
    char replacement = 0;
    if (ch < 0x80) {
      if (i == scheme.begin) {
        if (IsSchemeFirstChar(static_cast<unsigned char>(ch)))
          replacement = kSchemeCanonical[ch];
      } else {
        replacement = kSchemeCanonical[ch];
      }
    }

    if (replacement) {
      output->push_back(replacement);
    } else if (ch == '%') {
      success = false;
      output->push_back('%');
    } else {
      success = false;

      AppendUTF8EscapedChar(spec, &i, end, output);
    }
  }

  out_scheme->len = output->length() - out_scheme->begin;
  output->push_back(':');
  return success;
}
