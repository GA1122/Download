IDNConversionResult IDNToUnicodeWithAdjustments(
    base::StringPiece host,
    base::OffsetAdjuster::Adjustments* adjustments) {
  return IDNToUnicodeWithAdjustmentsImpl(host, adjustments, true);
}
