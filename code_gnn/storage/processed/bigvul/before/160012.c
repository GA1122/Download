bool BackendImpl::CreateBackingStore(disk_cache::File* file) {
  AdjustMaxCacheSize(0);

  IndexHeader header;
  header.table_len = DesiredIndexTableLen(max_size_);

  if (new_eviction_)
    header.version = 0x20001;

  header.create_time = Time::Now().ToInternalValue();

  if (!file->Write(&header, sizeof(header), 0))
    return false;

  return file->SetLength(GetIndexSize(header.table_len));
}
