bool FindMultipleClickBoundary(bool is_double_click, base::char16 cur) {
  if (!is_double_click)
    return cur == '\n';

  if (base::IsAsciiAlpha(cur) || base::IsAsciiDigit(cur) || cur == '_')
    return false;
  if (cur < 128)
    return true;

  static constexpr base::char16 kZeroWidthSpace = 0x200B;
  if (cur == kZeroWidthSpace)
    return true;

  return false;
}
