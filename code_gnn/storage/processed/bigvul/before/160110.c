void BackendImpl::UpgradeTo2_1() {
  DCHECK(0x20000 == data_->header.version);
  data_->header.version = 0x20001;
  data_->header.lru.sizes[Rankings::NO_USE] = data_->header.num_entries;
}
