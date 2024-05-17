IDNConversionResult UnsafeIDNToUnicodeWithDetails(base::StringPiece host) {
  return UnsafeIDNToUnicodeWithAdjustments(host, nullptr);
}
