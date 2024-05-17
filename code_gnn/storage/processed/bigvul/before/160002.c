void BackendImpl::AddStorageSize(int32_t bytes) {
  data_->header.num_bytes += bytes;
  DCHECK_GE(data_->header.num_bytes, 0);
}
