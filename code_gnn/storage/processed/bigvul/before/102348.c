string16 PolicyStatusInfo::GetPolicyLevelString(PolicyLevel level) {
  static const char* strings[] = { "mandatory", "recommended", "undefined" };
  DCHECK(static_cast<size_t>(level) < arraysize(strings));
  return ASCIIToUTF16(strings[level]);
}
