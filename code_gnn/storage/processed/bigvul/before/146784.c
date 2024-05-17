static inline bool IsValidNameStart(UChar32 c) {
  if ((c >= 0x02BB && c <= 0x02C1) || c == 0x559 || c == 0x6E5 || c == 0x6E6)
    return true;

  if (c == ':' || c == '_')
    return true;

  const uint32_t kNameStartMask =
      WTF::Unicode::kLetter_Lowercase | WTF::Unicode::kLetter_Uppercase |
      WTF::Unicode::kLetter_Other | WTF::Unicode::kLetter_Titlecase |
      WTF::Unicode::kNumber_Letter;
  if (!(WTF::Unicode::Category(c) & kNameStartMask))
    return false;

  if (c >= 0xF900 && c < 0xFFFE)
    return false;

  WTF::Unicode::CharDecompositionType decomp_type =
      WTF::Unicode::DecompositionType(c);
  if (decomp_type == WTF::Unicode::kDecompositionFont ||
      decomp_type == WTF::Unicode::kDecompositionCompat)
    return false;

  return true;
}
