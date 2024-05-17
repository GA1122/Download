static inline bool IsValidElementNamePerHTMLParser(const CharType* characters,
                                                   unsigned length) {
  CharType c = characters[0] | 0x20;
  if (!('a' <= c && c <= 'z'))
    return false;

  for (unsigned i = 1; i < length; ++i) {
    c = characters[i];
    if (c == '\t' || c == '\n' || c == '\f' || c == '\r' || c == ' ' ||
        c == '/' || c == '>')
      return false;
  }

  return true;
}
