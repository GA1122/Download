bool Document::IsValidName(const String& name) {
  unsigned length = name.length();
  if (!length)
    return false;

  if (name.Is8Bit()) {
    const LChar* characters = name.Characters8();

    if (IsValidNameASCII(characters, length))
      return true;

    return IsValidNameNonASCII(characters, length);
  }

  const UChar* characters = name.Characters16();

  if (IsValidNameASCII(characters, length))
    return true;

  return IsValidNameNonASCII(characters, length);
}
