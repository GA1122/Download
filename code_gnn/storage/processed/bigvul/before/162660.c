char CanonicalSchemeChar(base::char16 ch) {
  if (ch >= 0x80)
    return 0;   
  return kSchemeCanonical[ch];
}
