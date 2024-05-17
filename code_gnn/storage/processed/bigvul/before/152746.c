bool ReadHistogramArguments(PickleIterator* iter,
                            std::string* histogram_name,
                            int* flags,
                            int* declared_min,
                            int* declared_max,
                            uint32_t* bucket_count,
                            uint32_t* range_checksum) {
  if (!iter->ReadString(histogram_name) ||
      !iter->ReadInt(flags) ||
      !iter->ReadInt(declared_min) ||
      !iter->ReadInt(declared_max) ||
      !iter->ReadUInt32(bucket_count) ||
      !iter->ReadUInt32(range_checksum)) {
    DLOG(ERROR) << "Pickle error decoding Histogram: " << *histogram_name;
    return false;
  }

  if (*declared_max <= 0 ||
      *declared_min <= 0 ||
      *declared_max < *declared_min ||
      INT_MAX / sizeof(HistogramBase::Count) <= *bucket_count ||
      *bucket_count < 2) {
    DLOG(ERROR) << "Values error decoding Histogram: " << histogram_name;
    return false;
  }

  *flags &= ~HistogramBase::kIPCSerializationSourceFlag;

  return true;
}
