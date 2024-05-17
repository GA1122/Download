static ParseQualifiedNameResult ParseQualifiedNameInternal(
    const AtomicString& qualified_name,
    const CharType* characters,
    unsigned length,
    AtomicString& prefix,
    AtomicString& local_name) {
  bool name_start = true;
  bool saw_colon = false;
  unsigned colon_pos = 0;

  for (unsigned i = 0; i < length;) {
    UChar32 c;
    U16_NEXT(characters, i, length, c)
    if (c == ':') {
      if (saw_colon)
        return ParseQualifiedNameResult(kQNMultipleColons);
      name_start = true;
      saw_colon = true;
      colon_pos = i - 1;
    } else if (name_start) {
      if (!IsValidNameStart(c))
        return ParseQualifiedNameResult(kQNInvalidStartChar, c);
      name_start = false;
    } else {
      if (!IsValidNamePart(c))
        return ParseQualifiedNameResult(kQNInvalidChar, c);
    }
  }

  if (!saw_colon) {
    prefix = g_null_atom;
    local_name = qualified_name;
  } else {
    prefix = AtomicString(characters, colon_pos);
    if (prefix.IsEmpty())
      return ParseQualifiedNameResult(kQNEmptyPrefix);
    int prefix_start = colon_pos + 1;
    local_name = AtomicString(characters + prefix_start, length - prefix_start);
  }

  if (local_name.IsEmpty())
    return ParseQualifiedNameResult(kQNEmptyLocalName);

  return ParseQualifiedNameResult(kQNValid);
}
