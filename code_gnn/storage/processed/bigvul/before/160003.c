void BackendImpl::AdjustMaxCacheSize(int table_len) {
  if (max_size_)
    return;

  DCHECK(!table_len || data_->header.magic);

  int64_t available = base::SysInfo::AmountOfFreeDiskSpace(path_);
  if (available < 0) {
    max_size_ = kDefaultCacheSize;
    return;
  }

  if (table_len)
    available += data_->header.num_bytes;

  max_size_ = PreferredCacheSize(available);

  if (!table_len)
    return;

  max_size_ = std::min(max_size_, MaxStorageSizeForTable(table_len));
}
