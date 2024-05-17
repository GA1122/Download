void RecordSuffixedMemoryMBHistogram(base::StringPiece name,
                                     base::StringPiece suffix,
                                     int sample_mb) {
  std::string name_with_suffix;
  name.CopyToString(&name_with_suffix);
  suffix.AppendToString(&name_with_suffix);
  base::UmaHistogramMemoryMB(name_with_suffix, sample_mb);
}
