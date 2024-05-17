static bool IsValidElementNamePerHTMLParser(const String& name) {
  unsigned length = name.length();
  if (!length)
    return false;

  if (name.Is8Bit()) {
    const LChar* characters = name.Characters8();
    return IsValidElementNamePerHTMLParser(characters, length);
  }
  const UChar* characters = name.Characters16();
  return IsValidElementNamePerHTMLParser(characters, length);
}
