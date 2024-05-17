static bool IsJSNewline(UChar c) {
  return (c == '\n' || c == '\r' || c == 0x2028 || c == 0x2029);
}
