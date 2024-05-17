static inline bool IsValidNamePart(UChar32 c) {
  if (IsValidNameStart(c))
    return true;

  if (c == 0x00B7 || c == 0x0387)
    return true;

  if (c == '-' || c == '.')
    return true;

  const uint32_t kOtherNamePartMask =
      WTF::unicode::kMark_NonSpacing | WTF::unicode::kMark_Enclosing |
      WTF::unicode::kMark_SpacingCombining | WTF::unicode::kLetter_Modifier |
      WTF::unicode::kNumber_DecimalDigit;
  if (!(WTF::unicode::Category(c) & kOtherNamePartMask))
    return false;

  if (c >= 0xF900 && c < 0xFFFE)
    return false;

  WTF::unicode::CharDecompositionType decomp_type =
      WTF::unicode::DecompositionType(c);
  if (decomp_type == WTF::unicode::kDecompositionFont ||
      decomp_type == WTF::unicode::kDecompositionCompat)
    return false;

  return true;
}
