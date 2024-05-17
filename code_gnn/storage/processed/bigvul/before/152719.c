const std::string Histogram::GetAsciiBucketRange(uint32_t i) const {
  return GetSimpleAsciiBucketRange(ranges(i));
}
