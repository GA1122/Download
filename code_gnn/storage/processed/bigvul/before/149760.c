base::string16 IDNToUnicode(base::StringPiece host) {
  return IDNToUnicodeWithAdjustments(host, nullptr);
}
