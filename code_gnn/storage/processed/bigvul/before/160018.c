void BackendImpl::DecreaseNumEntries() {
  data_->header.num_entries--;
  if (data_->header.num_entries < 0) {
    NOTREACHED();
    data_->header.num_entries = 0;
  }
}
