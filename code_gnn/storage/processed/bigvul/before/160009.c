bool BackendImpl::CheckIndex() {
  DCHECK(data_);

  size_t current_size = index_->GetLength();
  if (current_size < sizeof(Index)) {
    LOG(ERROR) << "Corrupt Index file";
    return false;
  }

  if (new_eviction_) {
    if (kIndexMagic != data_->header.magic ||
        kCurrentVersion >> 16 != data_->header.version >> 16) {
      LOG(ERROR) << "Invalid file version or magic";
      return false;
    }
    if (kCurrentVersion == data_->header.version) {
      UpgradeTo2_1();
    }
  } else {
    if (kIndexMagic != data_->header.magic ||
        kCurrentVersion != data_->header.version) {
      LOG(ERROR) << "Invalid file version or magic";
      return false;
    }
  }

  if (!data_->header.table_len) {
    LOG(ERROR) << "Invalid table size";
    return false;
  }

  if (current_size < GetIndexSize(data_->header.table_len) ||
      data_->header.table_len & (kBaseTableLen - 1)) {
    LOG(ERROR) << "Corrupt Index file";
    return false;
  }

  AdjustMaxCacheSize(data_->header.table_len);

#if !defined(NET_BUILD_STRESS_CACHE)
  if (data_->header.num_bytes < 0 ||
      (max_size_ < std::numeric_limits<int32_t>::max() - kDefaultCacheSize &&
       data_->header.num_bytes > max_size_ + kDefaultCacheSize)) {
    LOG(ERROR) << "Invalid cache (current) size";
    return false;
  }
#endif

  if (data_->header.num_entries < 0) {
    LOG(ERROR) << "Invalid number of entries";
    return false;
  }

  if (!mask_)
    mask_ = data_->header.table_len - 1;

  return index_->Preload();
}
