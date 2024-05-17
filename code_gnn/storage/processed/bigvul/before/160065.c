void BackendImpl::ModifyStorageSize(int32_t old_size, int32_t new_size) {
  if (disabled_ || old_size == new_size)
    return;
  if (old_size > new_size)
    SubstractStorageSize(old_size - new_size);
  else
    AddStorageSize(new_size - old_size);

  FlushIndex();

  stats_.ModifyStorageStats(old_size, new_size);
}
