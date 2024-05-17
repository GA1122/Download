static bool IsValidNameNonASCII(const LChar* characters, unsigned length) {
  if (!IsValidNameStart(characters[0]))
    return false;

  for (unsigned i = 1; i < length; ++i) {
    if (!IsValidNamePart(characters[i]))
      return false;
  }

  return true;
}
