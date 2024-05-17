IDNConversionResult UnsafeIDNToUnicodeWithAdjustments(
    base::StringPiece host,
    base::OffsetAdjuster::Adjustments* adjustments) {
  return IDNToUnicodeWithAdjustmentsImpl(host, adjustments, false);
}
