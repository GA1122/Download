int DiskCacheBackendTest::GetEntryMetadataSize(std::string key) {
  if (!simple_cache_mode_)
    return key.size();

  return disk_cache::kSimpleEntryStreamCount *
         (sizeof(disk_cache::SimpleFileHeader) +
          sizeof(disk_cache::SimpleFileEOF) + key.size());
}
