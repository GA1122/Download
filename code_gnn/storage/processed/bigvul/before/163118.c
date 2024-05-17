size_t AppendDataInBuilder(BlobDataBuilder* builder,
                           size_t index,
                           disk_cache::Entry* cache_entry) {
  size_t size = 0;
  if (index % 2 != 0) {
    builder->AppendFutureData(5u);
    size += 5u;
    if (index % 3 == 1) {
      builder->AppendData("abcdefghij", 4u);
      size += 4u;
    }
    if (index % 3 == 0) {
      builder->AppendFutureData(1u);
      size += 1u;
    }
  } else if (index % 3 == 0) {
    builder->AppendFutureFile(0lu, 3lu, 0);
    size += 3u;
  }
  if (index % 5 != 0) {
    builder->AppendFile(
        base::FilePath::FromUTF8Unsafe(base::SizeTToString(index)), 0ul, 20ul,
        base::Time::Max());
    size += 20u;
  }
  if (index % 3 != 0) {
    scoped_refptr<BlobDataBuilder::DataHandle> disk_cache_data_handle =
        new EmptyDataHandle();
    builder->AppendDiskCacheEntry(disk_cache_data_handle, cache_entry,
                                  kTestDiskCacheStreamIndex);
    size += strlen(kTestDiskCacheData);
  }
  return size;
}
