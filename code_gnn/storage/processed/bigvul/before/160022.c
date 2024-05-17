int DesiredIndexTableLen(int32_t storage_size) {
  if (storage_size <= k64kEntriesStore)
    return kBaseTableLen;
  if (storage_size <= k64kEntriesStore * 2)
    return kBaseTableLen * 2;
  if (storage_size <= k64kEntriesStore * 4)
    return kBaseTableLen * 4;
  if (storage_size <= k64kEntriesStore * 8)
    return kBaseTableLen * 8;

  return kBaseTableLen * 16;
}
