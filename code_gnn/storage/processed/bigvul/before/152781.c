void SparseHistogram::WriteAsciiHeader(const Count total_count,
                                       std::string* output) const {
  StringAppendF(output,
                "Histogram: %s recorded %d samples",
                histogram_name().c_str(),
                total_count);
  if (flags())
    StringAppendF(output, " (flags = 0x%x)", flags());
}
