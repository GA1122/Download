static bool IsNonCanonicalCharacter(UChar c) {
  return (c == '\\' || c == '0' || c == '\0' || c == '/' || c == '?' ||
          c == '%' || c >= 127);
}
